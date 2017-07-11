
#include <pch.h>

#include <TurboGameState.h>
#include <Windows10Platform.h>
#include <Windows10GameController.h>

using namespace Windows::Foundation;

using namespace Turbo::Game;

#pragma region Constructors and Destructors ----------------------------------------------------------------------------

Turbo::Platform::Windows10::Windows10Platform::Windows10Platform(std::shared_ptr<ITurboGame> game, std::shared_ptr<ITurboGameRenderer> renderer) :
	_game(game),
	_renderer(renderer)
{
	_controller = std::unique_ptr<ITurboGameController>(new Windows10GameController());
}

Turbo::Platform::Windows10::Windows10Platform::~Windows10Platform()
{
}

#pragma endregion
#pragma region ITurboGamePlatform Methods ------------------------------------------------------------------------------

void Turbo::Platform::Windows10::Windows10Platform::Initialize()
{
	_game->Initialize();		//	Create level, scene & players
}

void Turbo::Platform::Windows10::Windows10Platform::Update()
{
	NavigationInfo navInfo = _controller->GetNavigationInfo();

	_game->Update(navInfo);

	if (_game->SceneChanged())
	{
		//_renderer = nullptr;
		_renderer->LoadSceneResources(_game->Scene());
	}
}

void Turbo::Platform::Windows10::Windows10Platform::Render()
{
	_renderer->RenderScene(_game->Scene());
}

void Turbo::Platform::Windows10::Windows10Platform::SaveState()
{
	std::shared_ptr<ITurboGameState> gameState = _game->GameState();
	SaveGameState(gameState);
}

void Turbo::Platform::Windows10::Windows10Platform::LoadState()
{
	std::shared_ptr<ITurboGameState> gameState = LoadGameState();
	_game->GameState(gameState);
}

void Turbo::Platform::Windows10::Windows10Platform::Finalize()
{
	_game->Finalize();
}

//void Turbo::Platform::Windows10::Windows10Platform::Resize(float width, float height)
//{
//	_renderer->Resize(width, height);
//}
//
//void Turbo::Platform::Windows10::Windows10Platform::SetDPI(float logicalDPI)
//{
//	_renderer->SetDPI(logicalDPI);
//}
//
//void Turbo::Platform::Windows10::Windows10Platform::SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation)
//{
//	_renderer->SetDisplayOrientation(displayOrientation);
//}
//
//void Turbo::Platform::Windows10::Windows10Platform::ValidateDevice()
//{
//	_renderer->ValidateDevice();
//}

#pragma endregion
#pragma region ITurboGamePlatform Properties ---------------------------------------------------------------------------

#pragma endregion
#pragma region Local Support Methods

void Turbo::Platform::Windows10::Windows10Platform::SaveGameState(std::shared_ptr<ITurboGameState> programState)
{

}

std::shared_ptr<ITurboGameState> Turbo::Platform::Windows10::Windows10Platform::LoadGameState()
{
	return std::shared_ptr<ITurboGameState>(new TurboGameState());
}

#pragma endregion
