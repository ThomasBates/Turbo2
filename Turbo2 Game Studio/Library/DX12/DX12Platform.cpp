
#include <pch.h>

#include <DX12Platform.h>
#include <DX12Renderer.h>
#include <DX12GameController.h>

#include <ITurboSceneMesh.h>

#include <TurboGameState.h>

using namespace Application_DX12;
using namespace Windows::Foundation;

#pragma region Constructors and Destructors ----------------------------------------------------------------------------

DX12Platform::DX12Platform(std::shared_ptr<ITurboGameLevel> program) :
	_level(program),
	_renderer(nullptr)
{
	_controller = std::unique_ptr<ITurboGameController>(new DX12GameController());
}

DX12Platform::~DX12Platform()
{
}

#pragma endregion
#pragma region ITurboGamePlatform Methods ------------------------------------------------------------------------------

void DX12Platform::Initialize()
{
	_level->Initialize();		//	Create level, scene & players
}

void DX12Platform::Update()
{
	NavigationInfo navInfo = _controller->GetNavigationInfo();

	_level->Update(navInfo);

	if (_level->SceneChanged())
	{
		//_renderer = nullptr;
		GetSceneRenderer()->LoadSceneResources(_level->Scene());
	}
}

void DX12Platform::Render()
{
	GetSceneRenderer()->RenderScene(_level->Scene());
}

void DX12Platform::SaveState()
{
	std::shared_ptr<ITurboGameState> gameState = _level->State();
	SaveGameState(gameState);
}

void DX12Platform::LoadState()
{
	std::shared_ptr<ITurboGameState> gameState = LoadGameState();
	_level->State(gameState);
}

void DX12Platform::Finalize()
{
	_level->Finalize();
}

void Application_DX12::DX12Platform::Resize(float width, float height)
{
	GetSceneRenderer()->Resize(width, height);
}

void Application_DX12::DX12Platform::SetDPI(float logicalDPI)
{
	GetSceneRenderer()->SetDPI(logicalDPI);
}

void Application_DX12::DX12Platform::SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation)
{
	GetSceneRenderer()->SetDisplayOrientation(displayOrientation);
}

void Application_DX12::DX12Platform::ValidateDevice()
{
	GetSceneRenderer()->ValidateDevice();
}

#pragma endregion
#pragma region ITurboGamePlatform Properties ---------------------------------------------------------------------------

#pragma endregion
#pragma region Local Support Methods

std::shared_ptr<DX12Renderer> DX12Platform::GetSceneRenderer()
{
	if ((_renderer != nullptr) && (_renderer->NeedsReset()))
	{
		_renderer = nullptr;
	}

	if (_renderer == nullptr)
	{
		_renderer = std::unique_ptr<DX12Renderer>(new DX12Renderer());
	}

	return _renderer;
}

void DX12Platform::SaveGameState(std::shared_ptr<ITurboGameState> programState)
{

}

std::shared_ptr<ITurboGameState> DX12Platform::LoadGameState()
{
	return std::shared_ptr<ITurboGameState>(new TurboGameState());
}

#pragma endregion
