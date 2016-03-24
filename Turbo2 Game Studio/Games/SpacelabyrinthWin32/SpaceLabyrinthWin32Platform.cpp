
//  Adapted from Jeff Molofee's tutorials at http://nehe.gamedev.net/

#include "pch.h"

#include "ApplicationWin32.h"
#include "SpaceLabyrinthWin32Platform.h"

#include "Bitmap.h"
#include "CanvasRGB.h"

#pragma region SpaceLabyrinthWin32Platform

#pragma region Constructors and Destructors

SpaceLabyrinthWin32Platform::SpaceLabyrinthWin32Platform(IApplication *application)
{
	_applicationWin32 = dynamic_cast<IApplicationWin32*>(application);
}

SpaceLabyrinthWin32Platform::~SpaceLabyrinthWin32Platform()
{
}

#pragma endregion
#pragma region ISpaceLabyrinthPlatform Methods

BOOL SpaceLabyrinthWin32Platform::Initialize(Camera *camera)
{
	_camera = camera;

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

	GLfloat LightAmbient[]= { 50.0f, 50.0f, 50.0f, 1.0f };	
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	
	GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 2);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 2);
	glEnable(GL_LIGHT1);

	_hRC = _applicationWin32->GetRenderingContext();
	_hDC = _applicationWin32->GetDeviceContext();

	QueryPerformanceFrequency(&_frequency);
	QueryPerformanceCounter(&_startCount);
	_lastCount	= _startCount;

	return TRUE;								// Everything Went OK
}

void SpaceLabyrinthWin32Platform::SetPlatformResources(IPlatformResources *platformResources)
{

}

BOOL SpaceLabyrinthWin32Platform::Resize(int width, int height)
{
	glViewport(0, 0, width-1, height-1);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(75.0f, (double)width/(double)height, 0.1f, 10000.0f);

	return TRUE;
}

BOOL SpaceLabyrinthWin32Platform::BeginDraw(int numCorners, int numEdges, int numWalls)
{
	_cornerCount = 0;
	_edgeCount = 0;
	_wallCount = 0;

	return TRUE;
}

BOOL SpaceLabyrinthWin32Platform::EndDraw()
{
	return TRUE;
}

BOOL SpaceLabyrinthWin32Platform::BeginUpdate()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	
	_time		= (float)((count.QuadPart - _startCount.QuadPart) / (double)_frequency.QuadPart);
	_deltaTime	= (float)((count.QuadPart - _lastCount.QuadPart ) / (double)_frequency.QuadPart);
	_lastCount	= count;

	return TRUE;
}

BOOL SpaceLabyrinthWin32Platform::EndUpdate()
{
	return TRUE;
}

BOOL SpaceLabyrinthWin32Platform::BeginRender()
{
	glMatrixMode(GL_MODELVIEW);			// Set the current matrix to be the model matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glLoadIdentity();							// Reset The Current Modelview Matrix

	glPushMatrix();

	gluLookAt(_camera->Position.X, _camera->Position.Y, _camera->Position.Z,
			  _camera->Target.X,   _camera->Target.Y,   _camera->Target.Z,
			  _camera->Up.X,       _camera->Up.Y,       _camera->Up.Z);

	return TRUE;
}

BOOL SpaceLabyrinthWin32Platform::EndRender()
{
	for (int i = 0; i < _cornerCount; i++)
		RedrawCorner(_corners[i]);
	for (int i = 0; i < _edgeCount; i++)
		RedrawEdge(_edges[i]);
	for (int i = 0; i < _wallCount; i++)
		RedrawWall(_walls[i]);

	glPopMatrix();

	SwapBuffers(_hDC);			// Swap Buffers (Double Buffering)

	return TRUE;
}

BOOL SpaceLabyrinthWin32Platform::Finalize()
{
	return TRUE;
}

BOOL SpaceLabyrinthWin32Platform::DrawCorner(MazeObject *corner)
{
	_corners[_cornerCount++] = corner;

	return TRUE;								// Keep Going
}

BOOL SpaceLabyrinthWin32Platform::DrawEdge(MazeObject *edge)
{
	_edges[_edgeCount++] = edge;

	return TRUE;								// Keep Going
}

BOOL SpaceLabyrinthWin32Platform::DrawWall(MazeObject *wall)
{
	_walls[_wallCount++] = wall;

	return TRUE;								// Keep Going
}

int SpaceLabyrinthWin32Platform::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo != NULL)
	{
		int x, y, status;

		_applicationWin32->GetPointer(&x, &y, &status);

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
		navInfo->MoveFore	= _applicationWin32->GetKey(VK_INSERT)	|| _applicationWin32->GetKey(VK_SPACE);
		navInfo->MoveBack	= _applicationWin32->GetKey(VK_DELETE)	|| _applicationWin32->GetKey('X');

		navInfo->PitchFore	= _applicationWin32->GetKey(VK_UP)		|| _applicationWin32->GetKey('W');
		navInfo->PitchBack	= _applicationWin32->GetKey(VK_DOWN)	|| _applicationWin32->GetKey('S');
		navInfo->YawRight	= _applicationWin32->GetKey(VK_RIGHT)	|| _applicationWin32->GetKey('D');
		navInfo->YawLeft	= _applicationWin32->GetKey(VK_LEFT)	|| _applicationWin32->GetKey('A');
		navInfo->RollLeft	= 0;
		navInfo->RollRight	= 0;

		navInfo->Restart	= _applicationWin32->GetKey(VK_F5); _applicationWin32->SetKey(VK_F5, FALSE);

		if (navInfo->MoveFore)
			navInfo->MoveFore = navInfo->MoveFore;

		return TRUE;
	}
	return FALSE;
}


#pragma endregion
#pragma region Local Support Methods

int SpaceLabyrinthWin32Platform::LoadTextures()
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

	LoadTexture(0, "..\\..\\Resources\\Textures\\stonefloor1.bmp");
	LoadTexture(1, "..\\..\\Resources\\Textures\\stonewall1.bmp");
	LoadTexture(2, "..\\..\\Resources\\Textures\\stonefloor3.bmp");	//	Floor
	LoadTexture(3, "..\\..\\Resources\\Textures\\stonefloor4.bmp");	//	Ceiling
	LoadTexture(4, "..\\..\\Resources\\Textures\\stonefloor5.bmp");
	LoadTexture(5, "..\\..\\Resources\\Textures\\stonefloor2.bmp");

	return status;								// Return The Status
}

int SpaceLabyrinthWin32Platform::LoadTexture(int id, const char *fileName)					// Loads A Bitmap Image
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

IImage *SpaceLabyrinthWin32Platform::LoadImage(const char *fileName)					// Loads A Bitmap Image
{
	IImage *image = new Bitmap(new CanvasRGB(0,0));
	image->LoadFromFile(fileName);
	image->Draw(256,256,IMG_ZOOM);
	return image;
}

void SpaceLabyrinthWin32Platform::SetCornerTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[3]);				// Select Our Texture
//	glColor3f(0,0,0);
}

void SpaceLabyrinthWin32Platform::SetEdgeTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[4]);				// Select Our Texture
//	glColor3f(0.1,0.1,0.1);
}

void SpaceLabyrinthWin32Platform::SetWallTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[1]);				// Select Our Texture
//	glColor3f(0.25,0.25,0.25);
}

void SpaceLabyrinthWin32Platform::SetCeilingTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[0]);				// Select Our Texture
//	glColor3f(0,0,0.5);
}

void SpaceLabyrinthWin32Platform::SetFloorTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture[2]);				// Select Our Texture
//	glColor3f(0,0.5,0);
}

BOOL SpaceLabyrinthWin32Platform::RedrawCorner(MazeObject *corner)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	SetCornerTexture();
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Left, corner->Bottom, corner->Back);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Back);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Right, corner->Top, corner->Back);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Left, corner->Top, corner->Back);	// Top Left Of The Texture and Quad
																					// Back Face
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Left, corner->Bottom, corner->Front);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Left, corner->Top, corner->Front);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Right, corner->Top, corner->Front);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Front);	// Bottom Left Of The Texture and Quad
																						// Top Face
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Left, corner->Top, corner->Front);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Left, corner->Top, corner->Back);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Right, corner->Top, corner->Back);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Right, corner->Top, corner->Front);	// Top Right Of The Texture and Quad
																						// Bottom Face
	glNormal3f(0, -1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Left, corner->Bottom, corner->Front);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Right, corner->Bottom, corner->Front);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Back);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Left, corner->Bottom, corner->Back);	// Bottom Right Of The Texture and Quad
																						// Right face
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Front);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Right, corner->Top, corner->Front);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Right, corner->Top, corner->Back);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Right, corner->Bottom, corner->Back);	// Bottom Left Of The Texture and Quad
																						// Left Face
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(corner->Left, corner->Bottom, corner->Front);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(corner->Left, corner->Bottom, corner->Back);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(corner->Left, corner->Top, corner->Back);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(corner->Left, corner->Top, corner->Front);	// Top Left Of The Texture and Quad
	glEnd();

	// Pop our Matrix from the stack (restore state)
	glPopMatrix();

	return TRUE;								// Keep Going
}

BOOL SpaceLabyrinthWin32Platform::RedrawEdge(MazeObject *edge)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	SetEdgeTexture();
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Left, edge->Bottom, edge->Back);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Back);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Right, edge->Top, edge->Back);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Left, edge->Top, edge->Back);	// Top Left Of The Texture and Quad
																				// Back Face
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Left, edge->Bottom, edge->Front);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Left, edge->Top, edge->Front);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Right, edge->Top, edge->Front);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Front);	// Bottom Left Of The Texture and Quad
																					// Top Face
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Left, edge->Top, edge->Front);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Left, edge->Top, edge->Back);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Right, edge->Top, edge->Back);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Right, edge->Top, edge->Front);	// Top Right Of The Texture and Quad
																				// Bottom Face
	glNormal3f(0, -1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Left, edge->Bottom, edge->Front);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Right, edge->Bottom, edge->Front);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Back);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Left, edge->Bottom, edge->Back);	// Bottom Right Of The Texture and Quad
																				// Right face
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Front);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Right, edge->Top, edge->Front);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Right, edge->Top, edge->Back);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Right, edge->Bottom, edge->Back);	// Bottom Left Of The Texture and Quad
																					// Left Face
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(edge->Left, edge->Bottom, edge->Front);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(edge->Left, edge->Bottom, edge->Back);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(edge->Left, edge->Top, edge->Back);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(edge->Left, edge->Top, edge->Front);	// Top Left Of The Texture and Quad
	glEnd();

	// Pop our Matrix from the stack (restore state)
	glPopMatrix();

	return TRUE;								// Keep Going
}

BOOL SpaceLabyrinthWin32Platform::RedrawWall(MazeObject *wall)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	if (wall->Back - wall->Front < wall->Top - wall->Bottom)
	{
		SetWallTexture();
		glBegin(GL_QUADS);
		// Front Face
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Left, wall->Bottom, wall->Back);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Back);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Right, wall->Top, wall->Back);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Left, wall->Top, wall->Back);	// Top Left Of The Texture and Quad
																					// Back Face
		glNormal3f(0, 0, -1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Left, wall->Bottom, wall->Front);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Left, wall->Top, wall->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Right, wall->Top, wall->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Front);	// Bottom Left Of The Texture and Quad
		glEnd();
	}

	if (wall->Top - wall->Bottom < wall->Right - wall->Left)
	{
		SetFloorTexture();
		glBegin(GL_QUADS);
		// Top Face
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Left, wall->Top, wall->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Left, wall->Top, wall->Back);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Right, wall->Top, wall->Back);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Right, wall->Top, wall->Front);	// Top Right Of The Texture and Quad
		glEnd();
		SetCeilingTexture();
		glBegin(GL_QUADS);
		// Bottom Face
		glNormal3f(0, -1, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Left, wall->Bottom, wall->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Right, wall->Bottom, wall->Front);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Back);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Left, wall->Bottom, wall->Back);	// Bottom Right Of The Texture and Quad
		glEnd();
	}

	if (wall->Right - wall->Left < wall->Back - wall->Front)
	{
		SetWallTexture();
		glBegin(GL_QUADS);
		// Right face
		glNormal3f(1, 0, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Front);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Right, wall->Top, wall->Front);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Right, wall->Top, wall->Back);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Right, wall->Bottom, wall->Back);	// Bottom Left Of The Texture and Quad
																						// Left Face
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->Left, wall->Bottom, wall->Front);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(wall->Left, wall->Bottom, wall->Back);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(wall->Left, wall->Top, wall->Back);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->Left, wall->Top, wall->Front);	// Top Left Of The Texture and Quad
		glEnd();
	}

	// Pop our Matrix from the stack (restore state)
	glPopMatrix();

	return TRUE;								// Keep Going
}

#pragma endregion

#pragma endregion
