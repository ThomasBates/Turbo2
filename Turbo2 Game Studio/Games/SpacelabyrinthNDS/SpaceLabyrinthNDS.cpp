
#include <math.h>

#include "Bitmap.h"
#include "CanvasRGB15.h"
#include "SpaceLabyrinthNDS.h"

#include "stonefloor1_bmp_bin.h"
#include "stonefloor2_bmp_bin.h"
#include "stonefloor3_bmp_bin.h"
#include "stonefloor4_bmp_bin.h"
#include "stonefloor5_bmp_bin.h"
#include "stonewall1_bmp_bin.h"
#include "stonewall2_bmp_bin.h"

#define TIMER_SPEED ((float)TIMER_HZ/(float)1024)

int colors = 1;


//  ============================================================================
//              SpaceLabyrinthNDS
//  ============================================================================


//============  Constructors and Destructors  --------------------------------------------------------------------------

SpaceLabyrinthNDS::SpaceLabyrinthNDS(ApplicationNDS *application)
{
	_application = application;
}

SpaceLabyrinthNDS::~SpaceLabyrinthNDS()
{
}

//----------------------------------------------------------------------------------------------------------------------
//============  ISpaceLabyrinthFactory Methods  ------------------------------------------------------------------------

int SpaceLabyrinthNDS::Initialize(Camera *camera)
{
	_camera = camera;
		
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);

	lcdMainOnBottom();

	// initialize the geometry engine
	glInit();
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(16,31,31,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	//ds specific, several attributes can be set here	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

	if (LoadTextures())
	{
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	}

	timerStart(0, ClockDivider_1024, 0, NULL);
	
	_ticks		= timerElapsed(0);

	return TRUE;						// Everything Went OK
}

int SpaceLabyrinthNDS::Resize(int width, int height)
{
	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	// setup the view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(75, 256.0 / 192.0, 0.1, 100);

	return TRUE;
}

int SpaceLabyrinthNDS::BeginUpdate()
{
	unsigned int ticks = timerElapsed(0);
	_ticks += ticks;
	
	_time = (float)_ticks / TIMER_SPEED;
	_deltaTime = (float)ticks / TIMER_SPEED;

	return TRUE;
}

int SpaceLabyrinthNDS::EndUpdate()
{
	return TRUE;
}

int SpaceLabyrinthNDS::BeginDraw()
{
	glMatrixMode(GL_MODELVIEW);			// Set the current matrix to be the model matrix
	glLoadIdentity();					// Reset The Current Modelview Matrix

	glPushMatrix();

	gluLookAt(_camera->Position.X, _camera->Position.Y, _camera->Position.Z,
			  _camera->Target.X,   _camera->Target.Y,   _camera->Target.Z,
			  _camera->Up.X,       _camera->Up.Y,       _camera->Up.Z);

	return TRUE;
}

int SpaceLabyrinthNDS::EndDraw()
{
	glPopMatrix(1);

	// flush to screen	
	glFlush(0);

	return TRUE;
}

int SpaceLabyrinthNDS::Finalize()
{
	timerStop(0);

	return TRUE;
}

int SpaceLabyrinthNDS::DrawCorner(MazeObject *corner)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	

	SetCornerTexture();
	glBegin(GL_QUADS);
		// Front Face
		SetVertex(corner->Left,  corner->Bottom, corner->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		SetVertex(corner->Right, corner->Bottom, corner->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(corner->Right, corner->Top,    corner->Back , 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(corner->Left,  corner->Top,    corner->Back , 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		// Back Face
		SetVertex(corner->Left,  corner->Bottom, corner->Front, 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(corner->Left,  corner->Top,    corner->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(corner->Right, corner->Top,    corner->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		SetVertex(corner->Right, corner->Bottom, corner->Front, 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		// Top Face
		SetVertex(corner->Left,  corner->Top,    corner->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		SetVertex(corner->Left,  corner->Top,    corner->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		SetVertex(corner->Right, corner->Top,    corner->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(corner->Right, corner->Top,    corner->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		// Bottom Face
		SetVertex(corner->Left,  corner->Bottom, corner->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(corner->Right, corner->Bottom, corner->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		SetVertex(corner->Right, corner->Bottom, corner->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		SetVertex(corner->Left,  corner->Bottom, corner->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		// Right face
		SetVertex(corner->Right, corner->Bottom, corner->Front, 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(corner->Right, corner->Top,    corner->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(corner->Right, corner->Top,    corner->Back , 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		SetVertex(corner->Right, corner->Bottom, corner->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		// Left Face
		SetVertex(corner->Left,  corner->Bottom, corner->Front, 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		SetVertex(corner->Left,  corner->Bottom, corner->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(corner->Left,  corner->Top,    corner->Back , 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(corner->Left,  corner->Top,    corner->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
	glEnd();                                                

	// Pop our Matrix from the stack (restore state)
	glPopMatrix(1);

	return TRUE;								// Keep Going
}

int SpaceLabyrinthNDS::DrawEdge(MazeObject *edge)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	

	SetEdgeTexture();
	glBegin(GL_QUADS);
		// Front Face
		SetVertex(edge->Left,  edge->Bottom, edge->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		SetVertex(edge->Right, edge->Bottom, edge->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(edge->Right, edge->Top,    edge->Back , 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(edge->Left,  edge->Top,    edge->Back , 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		// Back Face
		SetVertex(edge->Left,  edge->Bottom, edge->Front, 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(edge->Left,  edge->Top,    edge->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(edge->Right, edge->Top,    edge->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		SetVertex(edge->Right, edge->Bottom, edge->Front, 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		// Top Face
		SetVertex(edge->Left,  edge->Top,    edge->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		SetVertex(edge->Left,  edge->Top,    edge->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		SetVertex(edge->Right, edge->Top,    edge->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(edge->Right, edge->Top,    edge->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		// Bottom Face
		SetVertex(edge->Left,  edge->Bottom, edge->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(edge->Right, edge->Bottom, edge->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		SetVertex(edge->Right, edge->Bottom, edge->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		SetVertex(edge->Left,  edge->Bottom, edge->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		// Right face
		SetVertex(edge->Right, edge->Bottom, edge->Front, 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(edge->Right, edge->Top,    edge->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(edge->Right, edge->Top,    edge->Back , 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		SetVertex(edge->Right, edge->Bottom, edge->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		// Left Face
		SetVertex(edge->Left,  edge->Bottom, edge->Front, 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		SetVertex(edge->Left,  edge->Bottom, edge->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		SetVertex(edge->Left,  edge->Top,    edge->Back , 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		SetVertex(edge->Left,  edge->Top,    edge->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
	glEnd();                                          

	// Pop our Matrix from the stack (restore state)
	glPopMatrix(1);

	return TRUE;								// Keep Going
}

int SpaceLabyrinthNDS::DrawWall(MazeObject *wall)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	

	if (wall->Back - wall->Front < wall->Top - wall->Bottom)
	{
		SetWallTexture();
		glBegin(GL_QUADS);
			// Front Face
			SetVertex(wall->Left,  wall->Bottom, wall->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
			SetVertex(wall->Right, wall->Bottom, wall->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
			SetVertex(wall->Right, wall->Top,    wall->Back , 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
			SetVertex(wall->Left,  wall->Top,    wall->Back , 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
			// Back Face
			SetVertex(wall->Left,  wall->Bottom, wall->Front, 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
			SetVertex(wall->Left,  wall->Top,    wall->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
			SetVertex(wall->Right, wall->Top,    wall->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
			SetVertex(wall->Right, wall->Bottom, wall->Front, 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
		glEnd();                                          
	}

	if (wall->Top - wall->Bottom < wall->Right - wall->Left)
	{
		SetFloorTexture();
		glBegin(GL_QUADS);
			// Top Face
			SetVertex(wall->Left,  wall->Top,    wall->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
			SetVertex(wall->Left,  wall->Top,    wall->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
			SetVertex(wall->Right, wall->Top,    wall->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
			SetVertex(wall->Right, wall->Top,    wall->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
		glEnd();                                          
		SetCeilingTexture();
		glBegin(GL_QUADS);
			// Bottom Face
			SetVertex(wall->Left,  wall->Bottom, wall->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
			SetVertex(wall->Right, wall->Bottom, wall->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
			SetVertex(wall->Right, wall->Bottom, wall->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
			SetVertex(wall->Left,  wall->Bottom, wall->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
		glEnd();                                          
	}

	if (wall->Right - wall->Left < wall->Back - wall->Front)
	{
		SetWallTexture();
		glBegin(GL_QUADS);
			// Right face
			SetVertex(wall->Right, wall->Bottom, wall->Front, 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
			SetVertex(wall->Right, wall->Top,    wall->Front, 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
			SetVertex(wall->Right, wall->Top,    wall->Back , 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
			SetVertex(wall->Right, wall->Bottom, wall->Back , 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
			// Left Face
			SetVertex(wall->Left,  wall->Bottom, wall->Front, 0.0f, 0.0f); 	// Bottom Left Of The Texture and Quad
			SetVertex(wall->Left,  wall->Bottom, wall->Back , 1.0f, 0.0f); 	// Bottom Right Of The Texture and Quad
			SetVertex(wall->Left,  wall->Top,    wall->Back , 1.0f, 1.0f); 	// Top Right Of The Texture and Quad
			SetVertex(wall->Left,  wall->Top,    wall->Front, 0.0f, 1.0f); 	// Top Left Of The Texture and Quad
		glEnd();                                          
	}

	// Pop our Matrix from the stack (restore state)
	glPopMatrix(1);

	return TRUE;								// Keep Going
}

/*
from #include <nds\input.h>
//! enum values for the keypad buttons.
typedef enum KEYPAD_BITS {
  KEY_A      = BIT(0),  //!< Keypad A button.
  KEY_B      = BIT(1),  //!< Keypad B button.
  KEY_SELECT = BIT(2),  //!< Keypad SELECT button.
  KEY_START  = BIT(3),  //!< Keypad START button.
  KEY_RIGHT  = BIT(4),  //!< Keypad RIGHT button.
  KEY_LEFT   = BIT(5),  //!< Keypad LEFT button.
  KEY_UP     = BIT(6),  //!< Keypad UP button.
  KEY_DOWN   = BIT(7),  //!< Keypad DOWN button.
  KEY_R      = BIT(8),  //!< Right shoulder button.
  KEY_L      = BIT(9),  //!< Left shoulder button.
  KEY_X      = BIT(10), //!< Keypad X button.
  KEY_Y      = BIT(11), //!< Keypad Y button.
  KEY_TOUCH  = BIT(12), //!< Touchscreen pendown.
  KEY_LID    = BIT(13)  //!< Lid state.
} KEYPAD_BITS;
*/

int SpaceLabyrinthNDS::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo)
	{
		scanKeys();
		int keys = keysHeld();

		touchPosition touch;
		touchRead(&touch);

//		int keys = keysDown();
//		if (keys)
//			KeysDown(keys);

		navInfo->Pointer	= (keys & KEY_TOUCH);
		navInfo->PointerX	= touch.px;
		navInfo->PointerY	= touch.py;

		/*
		int ctrl = keys & (KEY_L|KEY_R);
		navInfo->MoveLeft	= !ctrl && (keys & KEY_LEFT);
		navInfo->MoveRight	= !ctrl && (keys & KEY_RIGHT);
		navInfo->MoveDown	= !ctrl && (keys & KEY_DOWN);
		navInfo->MoveUp		= !ctrl && (keys & KEY_UP);
		navInfo->MoveFore	= !ctrl && (keys & KEY_X);
		navInfo->MoveBack	= !ctrl && (keys & KEY_B);

		navInfo->PitchFore	=  ctrl && (keys & KEY_UP);
		navInfo->PitchBack	=  ctrl && (keys & KEY_DOWN);
		navInfo->YawRight	=  ctrl && (keys & KEY_RIGHT);
		navInfo->YawLeft	=  ctrl && (keys & KEY_LEFT);
		navInfo->RollLeft	=  ctrl && (keys & KEY_Y);
		navInfo->RollRight	=  ctrl && (keys & KEY_A);
		*/

		navInfo->MoveLeft	= 0;
		navInfo->MoveRight	= 0;
		navInfo->MoveDown	= 0;
		navInfo->MoveUp		= 0;
		navInfo->MoveFore	= (keys & KEY_L) || (keys & KEY_R); // keys & (KEY_L|KEY_R);
		navInfo->MoveBack	= keys & KEY_SELECT;

		navInfo->PitchFore	= keys & (KEY_UP    | KEY_X);
		navInfo->PitchBack	= keys & (KEY_DOWN  | KEY_B);
		navInfo->YawRight	= keys & (KEY_RIGHT | KEY_A);
		navInfo->YawLeft	= keys & (KEY_LEFT  | KEY_Y);
		navInfo->RollLeft	= 0;
		navInfo->RollRight	= 0;
		
		navInfo->Restart	= keys & KEY_START;

		return 1;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods  ---------------------------------------------------------------------------------

//-------------------------------------------------------
// set up a 2D layer constructed of bitmap sprites
// this holds the image when rendering to the top screen
//-------------------------------------------------------
void SpaceLabyrinthNDS::InitSubSprites()
{
	oamInit(&oamSub, SpriteMapping_Bmp_2D_256, false);
 
	int x = 0;
	int y = 0;
 
	int id = 0;

	//set up a 4x3 grid of 64x64 sprites to cover the screen
	for(y = 0; y < 3; y++)
	for(x = 0; x < 4; x++)
	{
		//u16 *offset = &SPRITE_GFX_SUB[(x * 64) + (y * 64 * 256)];
 
		//oamSet(&oamSub, x + y * 4, x * 64, y * 64, 0, 15, SpriteSize_64x64, 
		//	SpriteColorFormat_Bmp, offset, -1, false,false,false,false,false);
	
		oamSub.oamMemory[id].attribute[0] = ATTR0_BMP | ATTR0_SQUARE | (64 * y);
		oamSub.oamMemory[id].attribute[1] = ATTR1_SIZE_64 | (64 * x);
		oamSub.oamMemory[id].attribute[2] = ATTR2_ALPHA(1) | (8 * 32 * y) | (8 * x);
		id++;
	}
 
	swiWaitForVBlank();
 
	oamUpdate(&oamSub);
}

int SpaceLabyrinthNDS::LoadTextures()
{
	int status=TRUE;							// Status Indicator

	vramSetBankA(VRAM_A_TEXTURE);

	glGenTextures(6, _texture);					// Create The Texture

/*
#include "stonefloor1_bmp_bin.h"
#include "stonefloor2_bmp_bin.h"
#include "stonefloor3_bmp_bin.h"
#include "stonefloor4_bmp_bin.h"
#include "stonefloor5_bmp_bin.h"
#include "stonewall1_bmp_bin.h"
#include "stonewall2_bmp_bin.h"
*/

	LoadTexture(0, stonefloor1_bmp_bin);
	LoadTexture(1, stonewall1_bmp_bin);
	LoadTexture(2, stonefloor3_bmp_bin);	//	Floor
	LoadTexture(3, stonefloor3_bmp_bin);	//	Ceiling
	LoadTexture(4, stonefloor4_bmp_bin);
	LoadTexture(5, stonefloor2_bmp_bin);

	return status;								// Return The Status
}

int SpaceLabyrinthNDS::LoadTexture(int id, const u8 *imageData)					// Loads A Bitmap Image
{
	int Status=FALSE;							// Status Indicator

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	IImage *textureImage = LoadImage(imageData);
	if (textureImage)
	{
		Status=TRUE;							// Set The Status To TRUE

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, _texture[id]);

		// Generate The Texture
		void *data = textureImage->GetCanvas()->GetData();
		glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_64, TEXTURE_SIZE_64, 0, TEXGEN_TEXCOORD, data);
//		glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, data);
	}

	delete textureImage;						// Free The Image Structure

	return Status;								// Return The Status
}

IImage *SpaceLabyrinthNDS::LoadImage(const u8 *imageData)					// Loads A Bitmap Image
{
	IImage *image = new Bitmap(new CanvasRGB15(0,0));
	image->SetData((void*)imageData);
	image->Draw(64,64,IMG_ZOOM);
	return image;
}

void SpaceLabyrinthNDS::SetCornerTexture()
{
	if (colors)
		glColor3f(0,0,0);
	else
		glBindTexture(GL_TEXTURE_2D, _texture[3]);				// Select Our Texture
}

void SpaceLabyrinthNDS::SetEdgeTexture()
{
	if (colors)
		glColor3f(0.3,0.3,0.3);
	else
		glBindTexture(GL_TEXTURE_2D, _texture[4]);				// Select Our Texture
}

void SpaceLabyrinthNDS::SetWallTexture()
{
	if (colors)
		glColor3f(0.35,0.35,0.35);
	else
		glBindTexture(GL_TEXTURE_2D, _texture[1]);				// Select Our Texture
}

void SpaceLabyrinthNDS::SetCeilingTexture()
{
	if (colors)
		glColor3f(0,0,0.35);
	else
		glBindTexture(GL_TEXTURE_2D, _texture[0]);				// Select Our Texture
}

void SpaceLabyrinthNDS::SetFloorTexture()
{
	if (colors)
		glColor3f(0,0.35,0);
	else
		glBindTexture(GL_TEXTURE_2D, _texture[2]);				// Select Our Texture
}

void SpaceLabyrinthNDS::SetVertex(float x, float y, float z, float u, float v)
{
	glTexCoord2f(u, v);
	
//	float distance = (_camera->Position - Vector(x,y,z)).Length();
/*
	float distance = abs(_camera->Position.X - x)
	               + abs(_camera->Position.Y - y)
				   + abs(_camera->Position.Z - z);
*/
/*
	float shade = 2/distance;
	if (shade < 0.0) shade = 0.0;
	if (shade > 1.0) shade = 1.0;
	glColor3f(shade, shade, shade);
*/
	glVertex3f(x, y, z);
}

//----------------------------------------------------------------------------------------------------------------------
