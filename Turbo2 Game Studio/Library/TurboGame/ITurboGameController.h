
#pragma once

#include <TurboSceneTypes.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameController
		{
		public:
			virtual NavigationInfo GetNavigationInfo() = 0;
		};
	}
}