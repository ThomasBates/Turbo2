
#include "pch.h"

#include "IApplication.h"
#include "IApplicationDX12PlatformResources.h"

#include "Bitmap.h"
#include "CanvasRGB.h"
#include "TurboApplicationDX12Platform.h"
#include "TurboApplicationDX12Renderer.h"
#include "TurboApplicationDX12NavigationController.h"
#include "ITurboApplicationRenderer.h"

#include "TurboSceneObjectMesh.h"

using namespace Application_DX12;

#pragma region Constructors and Destructors

TurboApplicationDX12Platform::TurboApplicationDX12Platform(std::shared_ptr<IApplication> application)
{
	_controller = std::unique_ptr<INavigationController>(new TurboApplicationDX12NavigationController(application));
}

TurboApplicationDX12Platform::~TurboApplicationDX12Platform()
{
}

#pragma endregion
#pragma region ITurboApplicationPlatform Methods

int TurboApplicationDX12Platform::Initialize()
{
	return TRUE;								// Everything Went OK
}


void TurboApplicationDX12Platform::SetPlatformResources(std::shared_ptr<IPlatformResources> platformResources)
{
	if (platformResources == nullptr)
	{
		_sceneRenderer = nullptr;
	}
	else
	{
		std::shared_ptr<IApplicationDX12PlatformResources> dx12PlatformResources = std::dynamic_pointer_cast<IApplicationDX12PlatformResources>(platformResources);
		//_sceneRenderer = std::unique_ptr<ITurboApplicationRenderer>(new Sample3DSceneRenderer(dx12PlatformResources->GetDeviceResources()));
		_sceneRenderer = std::unique_ptr<ITurboApplicationRenderer>(new TurboApplicationDX12Renderer(dx12PlatformResources->GetDeviceResources()));
		Resize(0, 0);
	}
}

int TurboApplicationDX12Platform::Resize(int width, int height)
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	_sceneRenderer->Resize();

	return TRUE;
}

int TurboApplicationDX12Platform::BeginDraw()
{
	_sceneRenderer->BeginDraw();
	return TRUE;
}

int TurboApplicationDX12Platform::EndDraw()
{
	_sceneRenderer->EndDraw();
	return TRUE;
}

int TurboApplicationDX12Platform::BeginUpdate()
{
	return TRUE;
}

int TurboApplicationDX12Platform::EndUpdate()
{
	_sceneRenderer->Update();
	return TRUE;
}

int TurboApplicationDX12Platform::BeginRender()
{
	return TRUE;
}

int TurboApplicationDX12Platform::EndRender()
{
	// Don't try to render anything before the first Update.


	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	return _sceneRenderer->Render();

	return TRUE;
}

int TurboApplicationDX12Platform::Finalize()
{
	return TRUE;
}

std::shared_ptr<ITurboSceneObjectMesh> TurboApplicationDX12Platform::CreateMesh()
{
	return std::shared_ptr<ITurboSceneObjectMesh>(new TurboSceneObjectMesh());
}

std::shared_ptr<ITurboSceneObjectTexture> TurboApplicationDX12Platform::LoadTexture(std::string textureName)
{
	return nullptr;
}

void TurboApplicationDX12Platform::SetTimeStampForFrame()
{
	_controller->SetTimeStampForFrame();
}

NavigationInfo TurboApplicationDX12Platform::GetNavigationInfo()
{
	return _controller->GetNavigationInfo();
}

void TurboApplicationDX12Platform::RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	_sceneRenderer->RenderSceneObject(sceneObject);
}

#pragma endregion
#pragma region ITurboApplicationPlatform Properties

std::shared_ptr<ITurboSceneObjectPlacement> TurboApplicationDX12Platform::CameraPlacement()
{
	return _sceneRenderer->CameraPlacement();
}

void TurboApplicationDX12Platform::CameraPlacement(std::shared_ptr<ITurboSceneObjectPlacement> cameraPlacement)
{
	_sceneRenderer->CameraPlacement(cameraPlacement);
}

#pragma endregion
#pragma region Local Support Methods

int TurboApplicationDX12Platform::LoadTextures()
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

int TurboApplicationDX12Platform::LoadTexture(int id, std::string fileName)					// Loads A Bitmap Image
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

IImage *TurboApplicationDX12Platform::LoadImage(std::string fileName)					// Loads A Bitmap Image
{
	IImage *image = new Bitmap(new CanvasRGB(0,0));
	image->LoadFromFile(fileName);
	image->Draw(256,256,IMG_ZOOM);
	return image;
}

#pragma endregion
