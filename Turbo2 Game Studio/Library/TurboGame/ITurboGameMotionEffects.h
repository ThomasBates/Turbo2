
#pragma once

#include <pch.h>

#include <TurboSceneNavigationInfo.h>
#include <ITurboSceneObject.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameMotionEffects
		{
		public:
			//	ITurboGameMotionEffects Properties ---------------------------------------------------------------------

			//	ITurboGameMotionEffects Methods ------------------------------------------------------------------------
			virtual void ProcessMotionEffects(
				NavigationInfo* navInfo,
				std::shared_ptr<ITurboSceneObject> sceneObject,
				bool isPlayer) = 0;
		};
	}
}