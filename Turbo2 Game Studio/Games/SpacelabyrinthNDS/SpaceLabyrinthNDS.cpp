
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

//  ========================================================================  //
//  Constructors and Destructors
//  ========================================================================  //

SpaceLabyrinthNDS::SpaceLabyrinthNDS(ApplicationNDS *application)
{
	_application = application;
}

SpaceLabyrinthNDS::~SpaceLabyrinthNDS()
{
}

//  ========================================================================  //
//  ISpaceLabyrinthFactory Methods
//  ========================================================================  //


int SpaceLabyrinthNDS::Initialize()
{
	if (LoadTextures())
	{
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	}
	return TRUE;								// Everything Went OK
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

	xrot+=0.03f;								// X Axis Rotation
	yrot+=0.02f;								// Y Axis Rotation
	zrot+=0.04f;								// Z Axis Rotation
	return true;								// Keep Going
}

//  ========================================================================  //
//  Local Support Methods
//  ========================================================================  //


int SpaceLabyrinthNDS::LoadTextures()
{
	int status=TRUE;							// Status Indicator

	vramSetBankA(VRAM_A_TEXTURE);

	glGenTextures(6, texture);					// Create The Texture

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
		glBindTexture(GL_TEXTURE_2D, texture[id]);

		// Generate The Texture
		//int sizeX = textureImage->GetCanvas()->GetWidth();
		//int sizeY = textureImage->GetCanvas()->GetHeight();
		void *data = textureImage->GetCanvas()->GetData();
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, data);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, TEXGEN_TEXCOORD, data);

		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
	}

	delete textureImage;						// Free The Image Structure

	return Status;								// Return The Status
}

IImage *SpaceLabyrinthNDS::LoadImage(const u8 *imageData)					// Loads A Bitmap Image
{
	IImage *image = new Bitmap(new CanvasRGB15(0,0));
	image->SetData((void*)imageData);
	image->Draw(128,128,IMG_STRETCH);
	return image;
}


