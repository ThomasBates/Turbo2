
#include "pch.h"

#include "IApplication.h"
#include "IApplicationDX12.h"
#include "IApplicationDX12PlatformResources.h"

#include "Bitmap.h"
#include "CanvasRGB.h"
#include "SpaceLabyrinthDX12Platform.h"

using namespace Application_DX12;
using namespace SpaceLabyrinthDX12;

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


void SpaceLabyrinthDX12Platform::SetPlatformResources(IPlatformResources *platformResources)
{
	if (platformResources == nullptr)
	{
		m_sceneRenderer = nullptr;
	}
	else
	{
		IApplicationDX12PlatformResources *carrier = dynamic_cast<IApplicationDX12PlatformResources*>(platformResources);
		m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(carrier->GetDeviceResources()));
		Resize(0, 0);
	}
}

int SpaceLabyrinthDX12Platform::Resize(int width, int height)
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();

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

int SpaceLabyrinthDX12Platform::BeginUpdate()
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
	});

	return TRUE;
}

int SpaceLabyrinthDX12Platform::EndUpdate()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::BeginRender()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::EndRender()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return FALSE;
	}

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	return m_sceneRenderer->Render();

	return TRUE;
}

int SpaceLabyrinthDX12Platform::Finalize()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo != NULL)
	{
		//int x, y, status;

		//_applicationDX12->GetPointer(&x, &y, &status);

		navInfo->Pointer = 0; // status & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON);
		navInfo->PointerX = 0; // x;
		navInfo->PointerY = 0; // y;

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

		navInfo->MoveLeft = 0;
		navInfo->MoveRight = 0;
		navInfo->MoveDown = 0;
		navInfo->MoveUp = 0;
		navInfo->MoveFore = 0; // _applicationWin32->GetKey(VK_INSERT) || _applicationWin32->GetKey(VK_SPACE);
		navInfo->MoveBack = 0; // _applicationWin32->GetKey(VK_DELETE) || _applicationWin32->GetKey('X');

		navInfo->PitchFore = 0; // _applicationWin32->GetKey(VK_UP) || _applicationWin32->GetKey('W');
		navInfo->PitchBack = 0; // _applicationWin32->GetKey(VK_DOWN) || _applicationWin32->GetKey('S');
		navInfo->YawRight = 0; // _applicationWin32->GetKey(VK_RIGHT) || _applicationWin32->GetKey('D');
		navInfo->YawLeft = 0; // _applicationWin32->GetKey(VK_LEFT) || _applicationWin32->GetKey('A');
		navInfo->RollLeft = 0;
		navInfo->RollRight = 0;

		navInfo->Restart = 0; // _applicationWin32->GetKey(VK_F5); _applicationWin32->SetKey(VK_F5, FALSE);

		if (navInfo->MoveFore)
			navInfo->MoveFore = navInfo->MoveFore;

		return TRUE;
	}
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