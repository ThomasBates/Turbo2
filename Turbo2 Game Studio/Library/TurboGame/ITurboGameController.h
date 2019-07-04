
#pragma once

#include <TurboSceneNavigationInfo.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameController
		{
		public:
			virtual NavigationInfo* GetNavigationInfo() = 0;
			virtual void Suspend() = 0;
			virtual void Resume() = 0;
			virtual void ClearControls() = 0;
			virtual void AddControl(std::shared_ptr<ITurboSceneNavigationControl> control) = 0;
		};
	}
}