
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