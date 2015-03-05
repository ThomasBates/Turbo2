
//  Adapted from Jeff Molofee's tutorials at http://nehe.gamedev.net/

#include "Bitmap.h"
#include "CanvasRGB.h"
#include "SpaceLabyrinthWin32.h"

#pragma region Constructors and Destructors

SpaceLabyrinthWin32::SpaceLabyrinthWin32(ApplicationWin32 *application)
{
	_application = application;
}

SpaceLabyrinthWin32::~SpaceLabyrinthWin32()
{
}

#pragma endregion
#pragma region ISpaceLabyrinthFactory Methods

int SpaceLabyrinthWin32::Initialize()
{			
	glShadeModel(GL_SMOOTH);						// Enables Smooth Shading

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);					// White Background

	glClearDepth(1.0f);							// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations

	if (LoadTextures())
	{
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	}

	_hRC = _application->GetRenderingContext();
	_hDC = _application->GetDeviceContext();

	return TRUE;								// Everything Went OK
}

/*
int SpaceLabyrinthWin32::Reset()
{
	return Initialize();
}
*/

int SpaceLabyrinthWin32::Resize(int width, int height)
{
	if (width == 0)
		width = 1;
	if (height == 0)								// Prevent A Divide By Zero By
		height = 1;							// Making Height Equal One
	_width = width;
	_height = height;

	glViewport(0, 0, width-1, height-1);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(75.0f,(double)width/(double)height,0.1f,100.0f);

	return TRUE;
}

int SpaceLabyrinthWin32::BeginUpdate()
{
	glMatrixMode(GL_MODELVIEW);			// Set the current matrix to be the model matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glLoadIdentity();							// Reset The Current Modelview Matrix

	return TRUE;
}

int SpaceLabyrinthWin32::EndUpdate()
{
	SwapBuffers(_hDC);			// Swap Buffers (Double Buffering)

	return TRUE;
}

int SpaceLabyrinthWin32::Finalize()
{
	return TRUE;
}

BOOL SpaceLabyrinthWin32::DrawWall(float left, float top, float back, float right, float bottom, float front)
{
//GLfloat		xrot;								// X Rotation ( NEW )
//GLfloat		yrot;								// Y Rotation ( NEW )
//GLfloat		zrot;								// Z Rotation ( NEW )

	// Set the current matrix to be the model matrix
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1, 1, 1);									// Set the color..not in nehe source...ds gl default will be black
	//Push our original Matrix onto the stack (save state)
	glPushMatrix();	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glLoadIdentity();							// Reset The Current Modelview Matrix


	//glTranslatef(0.0f,0.0f,-5.0f);						// Move Into The Screen 5 Units

	//glRotatef(xrot,1.0f,0.0f,0.0f);						// Rotate On The X Axis
	//glRotatef(yrot,0.0f,1.0f,0.0f);						// Rotate On The Y Axis
	//glRotatef(zrot,0.0f,0.0f,1.0f);						// Rotate On The Z Axis

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
	glPopMatrix();

	//xrot+=0.03f;								// X Axis Rotation
	//yrot+=0.02f;								// Y Axis Rotation
	//zrot+=0.04f;								// Z Axis Rotation
	return TRUE;								// Keep Going
}

int SpaceLabyrinthWin32::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo != NULL)
	{
		int x, y, status;

		_application->GetPointer(&x, &y, &status);

		navInfo->Pointer		= status & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON);
		navInfo->PointerX	= x;
		navInfo->PointerY	= y;

		navInfo->MoveLeft	= _application->GetKey(VK_CONTROL) && _application->GetKey(VK_LEFT);
		navInfo->MoveRight	= _application->GetKey(VK_CONTROL) && _application->GetKey(VK_RIGHT);
		navInfo->MoveDown	= _application->GetKey(VK_CONTROL) && _application->GetKey(VK_UP);
		navInfo->MoveUp		= _application->GetKey(VK_CONTROL) && _application->GetKey(VK_DOWN);
		navInfo->MoveFore	= _application->GetKey(VK_CONTROL) && _application->GetKey(VK_INSERT);
		navInfo->MoveBack	= _application->GetKey(VK_CONTROL) && _application->GetKey(VK_DELETE);

		navInfo->PitchFore	=!_application->GetKey(VK_CONTROL) && _application->GetKey(VK_UP);
		navInfo->PitchBack	=!_application->GetKey(VK_CONTROL) && _application->GetKey(VK_DOWN);
		navInfo->YawRight	=!_application->GetKey(VK_CONTROL) && _application->GetKey(VK_RIGHT);
		navInfo->YawLeft		=!_application->GetKey(VK_CONTROL) && _application->GetKey(VK_LEFT);
		navInfo->RollLeft	=!_application->GetKey(VK_CONTROL) && _application->GetKey(VK_NUMPAD7);
		navInfo->RollRight	=!_application->GetKey(VK_CONTROL) && _application->GetKey(VK_NUMPAD9);

		return TRUE;
	}
	return FALSE;
}

int SpaceLabyrinthWin32::MoveCamera(float x, float y, float z)
{
	glMatrixMode(GL_PROJECTION);
	glTranslatef(x, y, z);
	return 1;
}

int SpaceLabyrinthWin32::RotateCamera(float x, float y, float z)
{
	glMatrixMode(GL_PROJECTION);
	glRotatef(x,1.0f,0.0f,0.0f);						// Rotate On The X Axis
	glRotatef(y,0.0f,1.0f,0.0f);						// Rotate On The Y Axis
	glRotatef(z,0.0f,0.0f,1.0f);						// Rotate On The Z Axis
	return 1;
}

#pragma endregion
#pragma region Local Support Methods

int SpaceLabyrinthWin32::LoadTextures()
{
	int status=TRUE;							// Status Indicator

	glGenTextures(6, _texture);					// Create The Texture

	LoadTexture(0, "Data\\test.bmp");
	LoadTexture(1, "Data\\test1.bmp");
	LoadTexture(2, "Data\\test2.bmp");
	LoadTexture(3, "Data\\Mandelbrot's Eye.bmp");
	LoadTexture(4, "Data\\BigHead.bmp");
	LoadTexture(5, "Data\\PokeBall.bmp");

	return status;								// Return The Status
}

int SpaceLabyrinthWin32::LoadTexture(int id, const char *fileName)					// Loads A Bitmap Image
{
	int Status=FALSE;							// Status Indicator

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	IImage *textureImage = LoadImage(fileName);
	if (textureImage)
	{
		Status=TRUE;							// Set The Status To TRUE

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, _texture[id]);

		// Generate The Texture
		void *data = textureImage->GetCanvas()->GetData();
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
	}

	delete textureImage;						// Free The Image Structure

	return Status;								// Return The Status
}

IImage *SpaceLabyrinthWin32::LoadImage(const char *fileName)					// Loads A Bitmap Image
{
	IImage *image = new Bitmap(new CanvasRGB(0,0));
	image->LoadFromFile(fileName);
	image->Draw(256,256,IMG_ZOOM);
	return image;
}

#pragma endregion