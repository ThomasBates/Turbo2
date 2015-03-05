
#include "Bitmap.h"
#include "CanvasRGB15.h"
#include "SpaceLabyrinthNDS.h"

#include "BIGHEAD_BMP_BIN.h"
#include "BIGHEAD2_BMP_BIN.h"
#include "MandelbrotEye_bmp_bin.h"
#include "POKEBALL_BMP_bin.h"
#include "test1_bmp_bin.h"
#include "test2_bmp_bin.h"
#include "test3_bmp_bin.h"
#include "test_bmp_bin.h"

#define TIMER_SPEED ((float)TIMER_HZ/(float)1024)

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

int SpaceLabyrinthNDS::Initialize()
{
		
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
		
	// initialize the geometry engine
	glInit();
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(31,31,31,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	//ds specific, several attributes can be set here	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

	if (LoadTextures())
	{
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	}

	timerStart(0, ClockDivider_1024, 0, NULL);


	return TRUE;						// Everything Went OK
}

/*
int SpaceLabyrinthNDS::Reset()
{
	return Initialize();
}
*/

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
	glMatrixMode(GL_MODELVIEW);			// Set the current matrix to be the model matrix
	glLoadIdentity();					// Reset The Current Modelview Matrix

	unsigned int ticks = timerElapsed(0);
	_ticks += ticks;
	
	_time = (float)_ticks / TIMER_SPEED;
	_deltaTime = (float)ticks / TIMER_SPEED;

	return TRUE;
}

int SpaceLabyrinthNDS::EndUpdate()
{
	//a handy little built in function to wait for a screen refresh
	//swiWaitForVBlank();

	// flush to screen	
	glFlush(0);

	return TRUE;
}

int SpaceLabyrinthNDS::Finalize()
{
	timerStop(0);

	return TRUE;
}

int SpaceLabyrinthNDS::DrawWall(float left, float top, float back, float right, float bottom, float front)
{
	glMatrixMode(GL_MODELVIEW);			// Set the current matrix to be the model matrix
	//Push our original Matrix onto the stack (save state)
	glPushMatrix();	

	glBindTexture(GL_TEXTURE_2D, _texture[0]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(left,  bottom, back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(right, bottom, back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(right, top,    back );	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(left,  top,    back );	// Top Left Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _texture[1]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(left,  bottom, front);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(left,  top,    front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(right, top,    front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(right, bottom, front);	// Bottom Left Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _texture[2]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(left,  top,    front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(left,  top,    back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(right, top,    back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(right, top,    front);	// Top Right Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _texture[3]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(left,  bottom, front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(right, bottom, front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(right, bottom, back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(left,  bottom, back );	// Bottom Right Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _texture[4]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(right, bottom, front);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(right, top,    front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(right, top,    back );	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(right, bottom, back );	// Bottom Left Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _texture[5]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(left,  bottom, front);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(left,  bottom, back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(left,  top,    back );	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(left,  top,    front);	// Top Left Of The Texture and Quad
	glEnd();

	// Pop our Matrix from the stack (restore state)
	glPopMatrix(1);

	return true;								// Keep Going
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

		int ctrl = keys & (KEY_L|KEY_R);
		
		navInfo->MoveLeft	= ctrl && (keys & KEY_LEFT);
		navInfo->MoveRight	= ctrl && (keys & KEY_RIGHT);
		navInfo->MoveDown	= ctrl && (keys & KEY_UP);
		navInfo->MoveUp		= ctrl && (keys & KEY_DOWN);
		navInfo->MoveFore	= ctrl && (keys & KEY_X);
		navInfo->MoveBack	= ctrl && (keys & KEY_B);

		navInfo->PitchFore	=!ctrl && (keys & KEY_UP);
		navInfo->PitchBack	=!ctrl && (keys & KEY_DOWN);
		navInfo->YawRight	=!ctrl && (keys & KEY_RIGHT);
		navInfo->YawLeft	=!ctrl && (keys & KEY_LEFT);
		navInfo->RollLeft	=!ctrl && (keys & KEY_Y);
		navInfo->RollRight	=!ctrl && (keys & KEY_A);

		return 1;
	}
	return 0;
}

int SpaceLabyrinthNDS::MoveCamera(float x, float y, float z)
{
	glMatrixMode(GL_PROJECTION);
	glTranslatef(x, y, z);
	return 1;
}

int SpaceLabyrinthNDS::RotateCamera(float x, float y, float z)
{
	glMatrixMode(GL_PROJECTION);
	glRotatef(x,1.0f,0.0f,0.0f);						// Rotate On The X Axis
	glRotatef(y,0.0f,1.0f,0.0f);						// Rotate On The Y Axis
	glRotatef(z,0.0f,0.0f,1.0f);						// Rotate On The Z Axis
	return 1;
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

	LoadTexture(0, test_bmp_bin);
	LoadTexture(1, test1_bmp_bin);
	LoadTexture(2, test2_bmp_bin);
	LoadTexture(3, MandelbrotEye_bmp_bin);
	LoadTexture(4, BIGHEAD_BMP_bin);
	LoadTexture(5, POKEBALL_BMP_bin);

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
//		glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_OFF, data);
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

//----------------------------------------------------------------------------------------------------------------------
