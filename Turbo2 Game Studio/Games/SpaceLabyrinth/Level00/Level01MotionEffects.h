
#pragma once

#include <pch.h>
#include <ITurboGameMotionEffects.h>

namespace Turbo
{
	namespace Game
	{
		class Level01MotionEffects : public ITurboGameMotionEffects
		{
		public:
			//  Constructors & Destructors -----------------------------------------------------------------------------
			Level01MotionEffects();

			//  ITurboGameMotionEffects Properties ---------------------------------------------------------------------

			//  ITurboGameMotionEffects Methods ------------------------------------------------------------------------
			virtual void ProcessMotionEffects(
				NavigationInfo navInfo,
				std::shared_ptr<ITurboSceneObject> sceneObject,
				bool isPlayer);

		private:
			const double cMoveAccelleration = 2.0f;
			const double cRotateAccelleration = 45.0f;
			const double cFrictionFactor = 2.0f;
			const double cHoverFrequency = 0.0f;
			const double cHoverMagnitude = 0.0f;
			const double cGravityFactor = 9.8f;
			const double cSelfRightingSpeed = 3000.0f;

			std::shared_ptr<ITurboGameMotionEffects> _baseMotionEffects;
		};
	}
}