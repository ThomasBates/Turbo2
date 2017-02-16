
#include <pch.h>

#include <IApplication.h>
#include <ITurboApplicationDX12Renderer.h>
#include <TurboApplicationDX12Platform.h>
#include <TurboApplicationDX12Renderer.h>
#include <TurboApplicationDX12NavigationController.h>

#include <ITurboSceneObjectMesh.h>

#include <ApplicationState.h>

using namespace Application_DX12;

#pragma region Constructors and Destructors  ---------------------------------------------------------------------------

TurboApplicationDX12Platform::TurboApplicationDX12Platform(std::shared_ptr<IProgram> program)
{
	_program = program;
	_controller = std::unique_ptr<INavigationController>(new TurboApplicationDX12NavigationController());
}

TurboApplicationDX12Platform::~TurboApplicationDX12Platform()
{
}

#pragma endregion
#pragma region ITurboApplicationPlatform Methods  ----------------------------------------------------------------------

void TurboApplicationDX12Platform::Initialize()
{
	_program->Initialize();		//	Create level, scene & players
}

void TurboApplicationDX12Platform::Update()
{
	NavigationInfo navInfo = _controller->GetNavigationInfo();

	_program->Update(navInfo);

	if (_program->NeedToRedrawStaticScene())
		_sceneRenderer->RenderStaticScene(_program->StaticScene());
}

void TurboApplicationDX12Platform::Render()
{
	// Don't try to render anything before the first Update.

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	if (_sceneRenderer->RenderDynamicScene(_program->DynamicScene()))
	{
		_deviceResources->Present();
	}
}

void TurboApplicationDX12Platform::SaveState()
{
	std::shared_ptr<IApplicationState> programState = _program->State();
	SaveProgramState(programState);
}

void TurboApplicationDX12Platform::LoadState()
{
	std::shared_ptr<IApplicationState> programState = LoadProgramState();
	_program->State(programState);
}

#pragma endregion
#pragma region ITurboApplicationPlatform Properties  -------------------------------------------------------------------

void TurboApplicationDX12Platform::DeviceResources(std::shared_ptr<DX::DeviceResources> deviceResources)
{
	_deviceResources = deviceResources;

	_sceneRenderer = nullptr;

	if (deviceResources != nullptr)
	{
		_sceneRenderer = std::unique_ptr<ITurboApplicationDX12Renderer>(new TurboApplicationDX12Renderer(deviceResources));
	}
}

#pragma endregion
#pragma region Local Support Methods

void TurboApplicationDX12Platform::SaveProgramState(std::shared_ptr<IApplicationState> programState)
{

}

std::shared_ptr<IApplicationState> TurboApplicationDX12Platform::LoadProgramState()
{
	return std::shared_ptr<IApplicationState>(new ApplicationState());
}

void TurboApplicationDX12Platform::LoadTextures()
{
	std::vector<std::shared_ptr<ITurboSceneObject>> sceneObjects = _program->StaticScene()->SceneObjects();

	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		std::shared_ptr<ITurboSceneObject> sceneObject = sceneObjects[i];
		std::string textureName = sceneObject->Material()->Texture()->Name();
		LoadTexture(textureName);
	}

	sceneObjects = _program->DynamicScene()->SceneObjects();

	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		std::shared_ptr<ITurboSceneObject> sceneObject = sceneObjects[i];
		std::string textureName = sceneObject->Material()->Texture()->Name();
		LoadTexture(textureName);
	}
}

void TurboApplicationDX12Platform::LoadTexture(std::string textureName)
{
	if (_loadedTextures.count(textureName) == 1)
		return;

	_loadedTextures[textureName] = "";
}

#pragma endregion
