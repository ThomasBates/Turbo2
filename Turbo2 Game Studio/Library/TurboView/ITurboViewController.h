
#pragma once

#include <pch.h>

#include <TurboSceneNavigationInfo.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace View
	{
		class ITurboViewController
		{
		public:
			virtual NavigationInfo *GetNavigationInfo() = 0;
			virtual void Suspend() = 0;
			virtual void Resume() = 0;
		};
	}
}