
#include "pch.h"

#include "IApplication.h"
#include "IApplicationDX12.h"
#include "IApplicationDX12PlatformResources.h"

#include "Bitmap.h"
#include "CanvasRGB.h"
#include "SpaceLabyrinthDX12Platform.h"
#include "SpaceLabyrinthDX12NavigationController.h"
#include "ISpaceLabyrinthRenderer.h"

using namespace Application_DX12;
using namespace SpaceLabyrinthDX12;

#pragma region Constructors and Destructors

SpaceLabyrinthDX12Platform::SpaceLabyrinthDX12Platform(IApplication *application)
{
	_applicationDX12 = dynamic_cast<IApplicationDX12*>(application);
	_controller = new SpaceLabyrinthDX12NavigationController(application);
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
		_sceneRenderer = nullptr;
	}
	else
	{
		IApplicationDX12PlatformResources *dx12Resources = dynamic_cast<IApplicationDX12PlatformResources*>(platformResources);
		//_sceneRenderer = std::unique_ptr<ISpaceLabyrinthRenderer>(new Sample3DSceneRenderer(carrier->GetDeviceResources()));
		_sceneRenderer = std::unique_ptr<ISpaceLabyrinthRenderer>(new SpaceLabyrinthDX12OriginalRenderer(dx12Resources->GetDeviceResources()));
		Resize(0, 0);
	}
}

int SpaceLabyrinthDX12Platform::Resize(int width, int height)
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	_sceneRenderer->Resize();

	return TRUE;
}

int SpaceLabyrinthDX12Platform::BeginDraw()
{
	_sceneRenderer->BeginDraw();
	return TRUE;
}

int SpaceLabyrinthDX12Platform::EndDraw()
{
	_sceneRenderer->EndDraw();
	return TRUE;
}

int SpaceLabyrinthDX12Platform::BeginUpdate()
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		_deltaTime = m_timer.GetElapsedSeconds();
	});

	return TRUE;
}

int SpaceLabyrinthDX12Platform::EndUpdate()
{
	_sceneRenderer->Update(_camera, _deltaTime);
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
	return _sceneRenderer->Render();

	return TRUE;
}

int SpaceLabyrinthDX12Platform::Finalize()
{
	return TRUE;
}

int SpaceLabyrinthDX12Platform::GetNavigationInfo(NavInfo *navInfo)
{
	return _controller->GetNavigationInfo(navInfo);
}

int SpaceLabyrinthDX12Platform::DrawCorner(MazeObject *corner)
{
	_sceneRenderer->DrawCorner(corner);
	return 1;
}

int SpaceLabyrinthDX12Platform::DrawEdge(MazeObject *edge)
{
	_sceneRenderer->DrawEdge(edge);
	return 1;
}

int SpaceLabyrinthDX12Platform::DrawWall(MazeObject *wall)
{
	_sceneRenderer->DrawWall(wall);
	return 1;
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
