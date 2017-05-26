#pragma once

#include "ITurboScene.h"

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneBuilder
		{
		public:
			//  ITurboSceneBuilder Methods -------------------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboScene> BuildScene() = 0;
		};
	}
}