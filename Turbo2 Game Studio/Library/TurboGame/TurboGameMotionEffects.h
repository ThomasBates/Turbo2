
#pragma once

#include <pch.h>
#include <ITurboGameMotionEffects.h>
#include <TurboSceneNavigationInfo.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace Game
	{
		class TurboGameMotionEffects : public ITurboGameMotionEffects
		{
		public:
			//  Constructors & Destructors -----------------------------------------------------------------------------
			TurboGameMotionEffects(
				double moveAcceleration,
				double rotateAcceleration,
				double frictionFactor,
				double hoverFrequency,
				double hoverMagnitude,
				double gravityFactor,
				double selfRightingSpeed) :
//				_moveAcceleration(moveAcceleration),
//				_rotateAcceleration(rotateAcceleration),
				_frictionFactor(frictionFactor),
				_hoverFrequency(hoverFrequency),
				_hoverMagnitude(hoverMagnitude),
				_gravityFactor(gravityFactor),
				_selfRightingSpeed(selfRightingSpeed)
			{
			}
			virtual ~TurboGameMotionEffects(){}

			//  ITurboGameMotionEffects Properties ---------------------------------------------------------------------

			//  ITurboGameMotionEffects Methods ------------------------------------------------------------------------
			virtual void ProcessMotionEffects(
				NavigationInfo* navInfo,
				std::shared_ptr<ITurboSceneObject> sceneObject,
				bool isPlayer);

		private:
//			double _moveAcceleration;
//			double _rotateAcceleration;
			double _frictionFactor;
			double _hoverFrequency;
			double _hoverMagnitude;
			double _gravityFactor;
			double _selfRightingSpeed;

			//NavigationInfo _lastNavInfo;
		};
	}
}