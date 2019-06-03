
#pragma once

#include <pch.h>

#include <ITurboGameLevel.h>
//#include <ITurboGameState.h>
//#include <ITurboGameController.h>

namespace Turbo
{
	namespace Game
	{
		class ITurboGame : public ITurboGameLevel
		{
		//public:
		//	//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
		//	virtual std::string Title() = 0;

		//	virtual std::shared_ptr<ITurboGameState> GameState() = 0;
		//	virtual void GameState(std::shared_ptr<ITurboGameState> gameState) = 0;

		//	virtual std::shared_ptr<ITurboGameLevel> Level() = 0;

		//	virtual bool LevelChanged() = 0;

		//	//	ITurboGameLevel Methods ----------------------------------------------------------------------------------------
		//	virtual void	Initialize() = 0;
		//	virtual void	Finalize() = 0;
		//	virtual void	Update(NavigationInfo* navInfo) = 0;
		};
	}
}