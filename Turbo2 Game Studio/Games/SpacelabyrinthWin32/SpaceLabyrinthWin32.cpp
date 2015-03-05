
//  Adapted from Jeff Molofee's tutorials at http://nehe.gamedev.net/

#include <math.h>

#include "ApplicationWin32.h"
#include "SpaceLabyrinthWin32.h"

#include "Bitmap.h"
#include "CanvasRGB.h"

#pragma region SpaceLabyrinthWin32

#pragma region Constructors and Destructors

SpaceLabyrinthWin32::SpaceLabyrinthWin32()
{
}

SpaceLabyrinthWin32::~SpaceLabyrinthWin32()
{
}

#pragma endregion
#pragma region ISpaceLabyrinthPlatform Methods

BOOL SpaceLabyrinthWin32::Initialize()
{			
	glShadeModel(GL_SMOOTH);						// Enables Smooth Shading

	glClearColor(0.5f, 1.0f, 1.0f, 1.0f);					// White Background

	glClearDepth(1.0f);							// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations

	if (LoadTextures())
	{
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	}

	_hRC = Win32Application->GetRenderingContext();
	_hDC = Win32Application->GetDeviceContext();

	QueryPerformanceFrequency(&_frequency);
	QueryPerformanceCounter(&_startCount);
	_lastCount	= _startCount;

	return TRUE;								// Everything Went OK
}

BOOL SpaceLabyrinthWin32::Resize(int width, int height)
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
	gluPerspective(75.0f,(double)width/(double)height,0.1f,10000.0f);

	return TRUE;
}

BOOL SpaceLabyrinthWin32::BeginUpdate()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	
	_time		= (float)((count.QuadPart - _startCount.QuadPart) / (double)_frequency.QuadPart);
	_deltaTime	= (float)((count.QuadPart - _lastCount.QuadPart ) / (double)_frequency.QuadPart);
	_lastCount	= count;

	return TRUE;
}

BOOL SpaceLabyrinthWin32::EndUpdate()
{
	return TRUE;
}

BOOL SpaceLabyrinthWin32::BeginDraw(const Camera &camera)
{
	glMatrixMode(GL_MODELVIEW);			// Set the current matrix to be the model matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glLoadIdentity();							// Reset The Current Modelview Matrix

	glPushMatrix();

	gluLookAt(camera.Position.X, camera.Position.Y, camera.Position.Z,
			  camera.Target.X, camera.Target.Y, camera.Target.Z,
			  camera.Up.X, camera.Up.Y, camera.Up.Z);

	return TRUE;
}

BOOL SpaceLabyrinthWin32::EndDraw()
{
	glPopMatrix();

	SwapBuffers(_hDC);			// Swap Buffers (Double Buffering)

	return TRUE;
}

BOOL SpaceLabyrinthWin32::Finalize()
{
	return TRUE;
}

BOOL SpaceLabyrinthWin32::DrawCorner(MazeObject *corner)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	

	SetCornerTexture();
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Left,  corner->Bottom, corner->Back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Right, corner->Top,    corner->Back );	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Left,  corner->Top,    corner->Back );	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Left,  corner->Bottom, corner->Front);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Left,  corner->Top,    corner->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Right, corner->Top,    corner->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Front);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Left,  corner->Top,    corner->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Left,  corner->Top,    corner->Back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Right, corner->Top,    corner->Back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Right, corner->Top,    corner->Front);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Left,  corner->Bottom, corner->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Right, corner->Bottom, corner->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Left,  corner->Bottom, corner->Back );	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Front);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Right, corner->Top,    corner->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Right, corner->Top,    corner->Back );	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Back );	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Left,  corner->Bottom, corner->Front);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Left,  corner->Bottom, corner->Back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Left,  corner->Top,    corner->Back );	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Left,  corner->Top,    corner->Front);	// Top Left Of The Texture and Quad
	glEnd();

	// Pop our Matrix from the stack (restore state)
	glPopMatrix();

	return TRUE;								// Keep Going
}

BOOL SpaceLabyrinthWin32::DrawEdge(MazeObject *edge)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	

	SetEdgeTexture();
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Left,  edge->Bottom, edge->Back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Right, edge->Top,    edge->Back );	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Left,  edge->Top,    edge->Back );	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Left,  edge->Bottom, edge->Front);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Left,  edge->Top,    edge->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Right, edge->Top,    edge->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Front);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Left,  edge->Top,    edge->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Left,  edge->Top,    edge->Back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Right, edge->Top,    edge->Back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Right, edge->Top,    edge->Front);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Left,  edge->Bottom, edge->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Right, edge->Bottom, edge->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Back );	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Left,  edge->Bottom, edge->Back );	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Front);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Right, edge->Top,    edge->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Right, edge->Top,    edge->Back );	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Back );	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Left,  edge->Bottom, edge->Front);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Left,  edge->Bottom, edge->Back );	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Left,  edge->Top,    edge->Back );	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Left,  edge->Top,    edge->Front);	// Top Left Of The Texture and Quad
	glEnd();

	// Pop our Matrix from the stack (restore state)
	glPopMatrix();

	return TRUE;								// Keep Going
}

BOOL SpaceLabyrinthWin32::DrawWall(MazeObject *wall)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	

	if (wall->Back - wall->Front < wall->Top - wall->Bottom)
	{
		SetWallTexture();
		glBegin(GL_QUADS);
			// Front Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Left,  wall->Bottom, wall->Back );	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Back );	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Right, wall->Top,    wall->Back );	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Left,  wall->Top,    wall->Back );	// Top Left Of The Texture and Quad
			// Back Face
			glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Left,  wall->Bottom, wall->Front);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Left,  wall->Top,    wall->Front);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Right, wall->Top,    wall->Front);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Front);	// Bottom Left Of The Texture and Quad
		glEnd();
	}

	if (wall->Top - wall->Bottom < wall->Right - wall->Left)
	{
		SetFloorTexture();
		glBegin(GL_QUADS);
			// Top Face
			glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Left,  wall->Top,    wall->Front);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Left,  wall->Top,    wall->Back );	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Right, wall->Top,    wall->Back );	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Right, wall->Top,    wall->Front);	// Top Right Of The Texture and Quad
		glEnd();
		SetCeilingTexture();
		glBegin(GL_QUADS);
			// Bottom Face
			glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Left,  wall->Bottom, wall->Front);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Right, wall->Bottom, wall->Front);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Back );	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Left,  wall->Bottom, wall->Back );	// Bottom Right Of The Texture and Quad
		glEnd();
	}

	if (wall->Right - wall->Left < wall->Back - wall->Front)
	{
		SetWallTexture();
		glBegin(GL_QUADS);
			// Right face
			glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Front);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Right, wall->Top,    wall->Front);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Right, wall->Top,    wall->Back );	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Back );	// Bottom Left Of The Texture and Quad
			// Left Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Left,  wall->Bottom, wall->Front);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Left,  wall->Bottom, wall->Back );	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Left,  wall->Top,    wall->Back );	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Left,  wall->Top,    wall->Front);	// Top Left Of The Texture and Quad
		glEnd();
	}

	// Pop our Matrix from the stack (restore state)
	glPopMatrix();

	return TRUE;								// Keep Going
}

int SpaceLabyrinthWin32::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo != NULL)
	{
		int x, y, status;

		Win32Application->GetPointer(&x, &y, &status);

		navInfo->Pointer	= status & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON);
		navInfo->PointerX	= x;
		navInfo->PointerY	= y;

		/*
		navInfo->MoveLeft	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_LEFT);
		navInfo->MoveRight	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_RIGHT);
		navInfo->MoveDown	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_DOWN);
		navInfo->MoveUp		=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_UP);
		navInfo->MoveFore	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_INSERT);
		navInfo->MoveBack	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_DELETE);

		navInfo->PitchFore	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_UP);
		navInfo->PitchBack	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_DOWN);
		navInfo->YawRight	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_RIGHT);
		navInfo->YawLeft	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_LEFT);
		navInfo->RollLeft	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_NUMPAD7);
		navInfo->RollRight	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_NUMPAD9);
		*/

		navInfo->MoveLeft	= 0;
		navInfo->MoveRight	= 0;
		navInfo->MoveDown	= 0;
		navInfo->MoveUp		= 0;
		navInfo->MoveFore	= Win32Application->GetKey(VK_INSERT)	|| Win32Application->GetKey(VK_SPACE);
		navInfo->MoveBack	= Win32Application->GetKey(VK_DELETE)	|| Win32Application->GetKey('X');

		navInfo->PitchFore	= Win32Application->GetKey(VK_UP)		|| Win32Application->GetKey('W');
		navInfo->PitchBack	= Win32Application->GetKey(VK_DOWN)		|| Win32Application->GetKey('S');
		navInfo->YawRight	= Win32Application->GetKey(VK_RIGHT)		|| Win32Application->GetKey('D');
		navInfo->YawLeft	= Win32Application->GetKey(VK_LEFT)		|| Win32Application->GetKey('A');
		navInfo->RollLeft	= 0;
		navInfo->RollRight	= 0;

		navInfo->Restart		= Win32Application->GetKey(VK_F5); Win32Application->SetKey(VK_F5, FALSE);

		return TRUE;
	}
	return FALSE;
}


#pragma endregion
#pragma region Local Support Methods

int SpaceLabyrinthWin32::LoadTextures()
{
	int status=TRUE;							// Status Indicator

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

	LoadTexture(0, "Data\\stonefloor1.bmp");
	LoadTexture(1, "Data\\stonewall1.bmp");
	LoadTexture(2, "Data\\stonefloor3.bmp");	//	Floor
	LoadTexture(3, "Data\\stonefloor4.bmp");	//	Ceiling
	LoadTexture(4, "Data\\stonefloor5.bmp");
	LoadTexture(5, "Data\\stonefloor2.bmp");

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

void SpaceLabyrinthWin32::SetCornerTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[3]);				// Select Our Texture
//	glColor3f(0,0,0);
}

void SpaceLabyrinthWin32::SetEdgeTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[4]);				// Select Our Texture
//	glColor3f(0.1,0.1,0.1);
}

void SpaceLabyrinthWin32::SetWallTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[1]);				// Select Our Texture
//	glColor3f(0.25,0.25,0.25);
}

void SpaceLabyrinthWin32::SetCeilingTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[0]);				// Select Our Texture
//	glColor3f(0,0,0.5);
}

void SpaceLabyrinthWin32::SetFloorTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[2]);				// Select Our Texture
//	glColor3f(0,0.5,0);
}

#pragma endregion

#pragma endregion
