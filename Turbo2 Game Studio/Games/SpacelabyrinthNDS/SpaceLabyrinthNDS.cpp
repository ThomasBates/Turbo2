
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
	glClearColor(1,1,1,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	// setup the view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
	
	
	//ds specific, several attributes can be set here	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

	if (LoadTextures())
	{
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	}

/*
	// initialize gl
	glInit();
 
	// sub sprites hold the bottom image when 3D directed to top
	InitSubSprites();
 
	// sub background holds the top image when 3D directed to bottom
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	
	//enable textures
	glEnable(GL_TEXTURE_2D);
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call
	glViewport(0,0,255,191);
	
	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	
	// setup other material properties
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(20,20,20));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(5,5,5));
	
	
	// setup the lighting
	glLight(0, RGB15(31,31,31) , 0, floattov10(-.5), floattov10(-.85));

	InitTextures();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	gluLookAt(	0.0, 0.0, 1.0,		//camera possition 
				0.0, 0.0,-2.0,		//look at
				0.0, 1.0, 0.0);		//up
	
	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0 | POLY_ID(1) ) ;
	
	glColor3f(1,1,1);

	glMatrixMode(GL_MODELVIEW);


	//glShadeModel(GL_SMOOTH);						// Enables Smooth Shading

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// Black Background

	glClearDepth(1.0f);							// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations


	if (LoadTextures())
	{
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	}
*/
	return TRUE;								// Everything Went OK
}

int SpaceLabyrinthNDS::BeginUpdate()
{
	// Set the current matrix to be the model matrix
	glMatrixMode(GL_MODELVIEW);
	
	glColor3f(1, 1, 1);									// Set the color..not in nehe source...ds gl default will be black
	
	//Push our original Matrix onto the stack (save state)
	glPushMatrix();	

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glLoadIdentity();							// Reset The Current Modelview Matrix

	return TRUE;
}

int SpaceLabyrinthNDS::EndUpdate()
{
	// Pop our Matrix from the stack (restore state)
	glPopMatrix(1);

	//a handy little built in function to wait for a screen refresh
	swiWaitForVBlank();

	// flush to screen	
	glFlush(0);

	return TRUE;
}

int SpaceLabyrinthNDS::Resize(int width, int height)
{
	return TRUE;
}

int SpaceLabyrinthNDS::Reset()
{
	return Initialize();
}

int SpaceLabyrinthNDS::Finalize()
{
	return TRUE;
}

float		xrot;								// X Rotation ( NEW )
float		yrot;								// Y Rotation ( NEW )
float		zrot;								// Z Rotation ( NEW )

int			texture[6];							// Storage For One Texture ( NEW )

int SpaceLabyrinthNDS::DrawWall(double left, double top, double back, double right, double bottom, double front)
{
	glTranslatef(0.0f,0.0f,-5.0f);						// Move Into The Screen 5 Units

	glRotatef(xrot,1.0f,0.0f,0.0f);						// Rotate On The X Axis
	glRotatef(yrot,0.0f,1.0f,0.0f);						// Rotate On The Y Axis
	glRotatef(zrot,0.0f,0.0f,1.0f);						// Rotate On The Z Axis

	glBindTexture(GL_TEXTURE_2D, texture[0]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[1]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[2]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[3]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[4]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[5]);				// Select Our Texture
	glBegin(GL_QUADS);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glEnd();

	xrot+=0.3f;								// X Axis Rotation
	yrot+=0.2f;								// Y Axis Rotation
	zrot+=0.4f;								// Z Axis Rotation
	return true;								// Keep Going
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

	glGenTextures(6, texture);					// Create The Texture

	LoadTexture(0, test_bmp_bin);
	LoadTexture(1, test1_bmp_bin);
	LoadTexture(2, BIGHEAD_BMP_bin);
	LoadTexture(3, POKEBALL_BMP_bin);
	LoadTexture(4, test2_bmp_bin);
	LoadTexture(5, MandelbrotEye_bmp_bin);

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
		glBindTexture(GL_TEXTURE_2D, texture[id]);

		// Generate The Texture
		void *data = textureImage->GetCanvas()->GetData();
		glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, data);
	}

	delete textureImage;						// Free The Image Structure

	return Status;								// Return The Status
}

IImage *SpaceLabyrinthNDS::LoadImage(const u8 *imageData)					// Loads A Bitmap Image
{
	IImage *image = new Bitmap(new CanvasRGB15(0,0));
	image->SetData((void*)imageData);
	image->Draw(128,128,IMG_ZOOM);
	return image;
}

//----------------------------------------------------------------------------------------------------------------------
