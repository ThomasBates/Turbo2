
#include <pch.h>

#include <ITurboApplicationDX12Renderer.h>
#include <TurboApplicationDX12Platform.h>
#include <TurboApplicationDX12Renderer.h>
#include <TurboApplicationDX12NavigationController.h>

#include <ITurboSceneMesh.h>

#include <ApplicationState.h>

using namespace Application_DX12;
using namespace Windows::Foundation;

#pragma region Constructors and Destructors  ---------------------------------------------------------------------------

TurboApplicationDX12Platform::TurboApplicationDX12Platform(std::shared_ptr<IGameLevel> program) :
	_program(program),
	_sceneRenderer(nullptr)
{
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

	if (_program->SceneChanged())
	{
		//_sceneRenderer = nullptr;
		GetSceneRenderer()->LoadSceneResources(_program->Scene());
	}
}

void TurboApplicationDX12Platform::Render()
{
	GetSceneRenderer()->RenderScene(_program->Scene());
}

void TurboApplicationDX12Platform::SaveState()
{
	std::shared_ptr<IApplicationState> gameState = _program->State();
	SaveGameState(gameState);
}

void TurboApplicationDX12Platform::LoadState()
{
	std::shared_ptr<IApplicationState> gameState = LoadGameState();
	_program->State(gameState);
}

void TurboApplicationDX12Platform::Finalize()
{
	_program->Finalize();
}

void Application_DX12::TurboApplicationDX12Platform::Resize(float width, float height)
{
	GetSceneRenderer()->Resize(width, height);
}

void Application_DX12::TurboApplicationDX12Platform::SetDPI(float logicalDPI)
{
	GetSceneRenderer()->SetDPI(logicalDPI);
}

void Application_DX12::TurboApplicationDX12Platform::SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation)
{
	GetSceneRenderer()->SetDisplayOrientation(displayOrientation);
}

void Application_DX12::TurboApplicationDX12Platform::ValidateDevice()
{
	GetSceneRenderer()->ValidateDevice();
}

#pragma endregion
#pragma region ITurboApplicationPlatform Properties  -------------------------------------------------------------------

#pragma endregion
#pragma region Local Support Methods

std::shared_ptr<ITurboApplicationDX12Renderer> TurboApplicationDX12Platform::GetSceneRenderer()
{
	if ((_sceneRenderer != nullptr) && (_sceneRenderer->NeedsReset()))
	{
		_sceneRenderer = nullptr;
	}

	if (_sceneRenderer == nullptr)
	{
		_sceneRenderer = std::unique_ptr<ITurboApplicationDX12Renderer>(new TurboApplicationDX12Renderer());
	}

	return _sceneRenderer;
}

void TurboApplicationDX12Platform::SaveGameState(std::shared_ptr<IApplicationState> programState)
{

}

std::shared_ptr<IApplicationState> TurboApplicationDX12Platform::LoadGameState()
{
	return std::shared_ptr<IApplicationState>(new ApplicationState());
}

#pragma endregion
