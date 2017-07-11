
#pragma once

#include <pch.h>

#include <ITurboGameController.h>
#include <ITurboGameState.h>
#include <ITurboScene.h>

#include <TurboGameTypes.h>

namespace Turbo
{
	namespace Game
	{
		class ITurboGameLevel
		{
		public:
			//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
			virtual std::string Title() = 0;

			virtual std::shared_ptr<ITurboGameState> GameState() = 0;
			virtual void GameState(std::shared_ptr<ITurboGameState> gameState) = 0;

			virtual TurboGameLevelState LevelState() = 0;
			virtual void LevelState(TurboGameLevelState levelState) = 0;

			virtual std::shared_ptr<ITurboScene> Scene() = 0;
			virtual std::shared_ptr<ITurboSceneObject> Player() = 0;

			virtual bool SceneChanged() = 0;

			//	ITurboGameLevel Methods ----------------------------------------------------------------------------------------
			virtual void	Initialize() = 0;
			virtual void	Finalize() = 0;
			virtual void	Update(NavigationInfo navInfo) = 0;
		};
	}
}