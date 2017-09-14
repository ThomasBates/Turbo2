
#pragma once

#include <ITurboGame.h>
#include <ITurboGameController.h>
#include <ITurboGamePlatform.h>
#include <ITurboGameRenderer.h>
#include <ITurboGameState.h>

using namespace Turbo::Game;

namespace Turbo
{
	namespace Platform
	{
		namespace Windows10
		{
			class Windows10Platform : public ITurboGamePlatform
			{
			public:
				//  Constructors and Destructors -------------------------------------------------------------------------------
				Windows10Platform(std::shared_ptr<ITurboGame> game, std::shared_ptr<ITurboGameRenderer> renderer);
				~Windows10Platform();

				//  Methods ----------------------------------------------------------------------------------------------------
				virtual void Initialize();
				virtual void Update();
				virtual void Render();
				virtual void SaveState();
				virtual void LoadState();
				virtual void Finalize();

				//virtual void Resize(float width, float height);
				//virtual void SetDPI(float logicalDPI);
				//virtual void SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation);
				//virtual void ValidateDevice();

			private:
				std::shared_ptr<ITurboGame>				_game;
				std::shared_ptr<ITurboGameRenderer>		_renderer;
				std::unique_ptr<ITurboGameController>	_controller;

				//  Local Support Methods --------------------------------------------------------------------------------------
				std::shared_ptr<ITurboGameRenderer> GetSceneRenderer();

				void SaveGameState(std::shared_ptr<ITurboGameState> gameState);
				std::shared_ptr<ITurboGameState> LoadGameState();
			};
		}
	}
}