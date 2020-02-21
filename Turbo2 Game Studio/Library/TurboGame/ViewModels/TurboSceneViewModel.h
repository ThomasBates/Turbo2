
#pragma once

#include <pch.h>

#include <ITurboGameLevel.h>
#include <ITurboScene.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace Game
	{
		class TurboSceneViewModel
		{
		private:
			std::shared_ptr<ITurboGameLevel> _level;

		public:
			//  Constructors and Destructors -----------------------------------------------------------------------------------
			TurboSceneViewModel(std::shared_ptr<ITurboGameLevel> level);
			virtual ~TurboSceneViewModel(){}

			//  Public Properties -----------------------------------------------------------------------------------------
			std::shared_ptr<ITurboScene> Scene();
		};
	}
}
