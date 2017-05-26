
#pragma once

#include <pch.h>

#include <ITurboGameState.h>
#include <ITurboGameController.h>
#include <ITurboScene.h>

namespace Turbo
{
	namespace Game
	{
		class ITurboGameLevel
		{
		public:
			//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
			virtual std::string Title() = 0;

			virtual std::shared_ptr<ITurboGameState> State() = 0;
			virtual void State(std::shared_ptr<ITurboGameState> state) = 0;

			virtual std::shared_ptr<ITurboScene> Scene() = 0;

			virtual bool SceneChanged() = 0;

			//	ITurboGameLevel Methods ----------------------------------------------------------------------------------------
			virtual void	Initialize() = 0;
			virtual void	Finalize() = 0;
			virtual void	Update(NavigationInfo navInfo) = 0;
		};
	}
}