
#pragma once

#include <pch.h>
#include <ITurboGameMotionEffects.h>
#include <TurboSceneTypes.h>

namespace Turbo
{
	namespace Game
	{
		class TurboGameMotionEffects : public ITurboGameMotionEffects
		{
		public:
			//  Constructors & Destructors -----------------------------------------------------------------------------
			TurboGameMotionEffects(
				double moveAccelleration,
				double rotateAccelleration,
				double frictionFactor,
				double hoverFrequency,
				double hoverMagnitude,
				double gravityFactor,
				double selfRightingSpeed) :
				//_moveAccelleration(moveAccelleration),
				//_rotateAccelleration(rotateAccelleration),
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
			//double _moveAccelleration;
			//double _rotateAccelleration;
			double _frictionFactor;
			double _hoverFrequency;
			double _hoverMagnitude;
			double _gravityFactor;
			double _selfRightingSpeed;

			NavigationInfo _lastNavInfo;
		};
	}
}