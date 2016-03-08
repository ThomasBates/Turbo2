
#include "pch.h"

#include "IApplication.h"
#include "Bitmap.h"
#include "CanvasRGB.h"
#include "SpaceLabyrinthDX12Platform.h"

#pragma region Constructors and Destructors

SpaceLabyrinthDX12Platform::SpaceLabyrinthDX12Platform(IApplication *application)
{
	_applicationDX12 = dynamic_cast<IApplicationDX12*>(application);
}

SpaceLabyrinthDX12Platform::~SpaceLabyrinthDX12Platform()
{
}

#pragma endregion
#pragma region ISpaceLabyrinthFactory Methods

int SpaceLabyrinthDX12Platform::Initialize(Camera *camera)
{			
	_camera = camera;

	return TRUE;								// Everything Went OK
}

int SpaceLabyrinthDX12Platform::BeginUpdate()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::EndUpdate()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::BeginDraw()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::EndDraw()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::Finalize()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::GetNavigationInfo(NavInfo *navInfo)
{
	return FALSE;
}

int SpaceLabyrinthDX12Platform::DrawCorner(MazeObject *corner)
{
	return FALSE;
}

int SpaceLabyrinthDX12Platform::DrawEdge(MazeObject *edge)
{
	return FALSE;
}

int SpaceLabyrinthDX12Platform::DrawWall(MazeObject *wall)
{
	return FALSE;
}


#pragma endregion
#pragma region Local Support Methods

int SpaceLabyrinthDX12Platform::LoadTextures()
{
	int status=TRUE;							// Status Indicator

	LoadTexture(0, "Data\\test.bmp");
	LoadTexture(1, "Data\\test1.bmp");
	LoadTexture(2, "Data\\test2.bmp");
	LoadTexture(3, "Data\\Mandelbrot's Eye.bmp");
	LoadTexture(4, "Data\\BigHead.bmp");
	LoadTexture(5, "Data\\PokeBall.bmp");

	return status;								// Return The Status
}

int SpaceLabyrinthDX12Platform::LoadTexture(int id, const char *fileName)					// Loads A Bitmap Image
{
	int Status=FALSE;							// Status Indicator

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	IImage *textureImage = LoadImage(fileName);
	if (textureImage)
	{
		Status=TRUE;							// Set The Status To TRUE
	}

	delete textureImage;						// Free The Image Structure

	return Status;								// Return The Status
}

IImage *SpaceLabyrinthDX12Platform::LoadImage(const char *fileName)					// Loads A Bitmap Image
{
	IImage *image = new Bitmap(new CanvasRGB(0,0));
	image->LoadFromFile(fileName);
	image->Draw(256,256,IMG_ZOOM);
	return image;
}

#pragma endregion