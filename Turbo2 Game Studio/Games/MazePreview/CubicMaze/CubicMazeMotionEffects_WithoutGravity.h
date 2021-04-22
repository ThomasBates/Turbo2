
#pragma once

#include <pch.h>
#include <ITurboGameMotionEffects.h>

namespace Turbo
{
	namespace Game
	{
		class CubicMazeMotionEffects_WithoutGravity : public ITurboGameMotionEffects
		{
		public:
			//  Constructors & Destructors -----------------------------------------------------------------------------
			CubicMazeMotionEffects_WithoutGravity();
			virtual ~CubicMazeMotionEffects_WithoutGravity(){}

			//  ITurboGameMotionEffects Properties ---------------------------------------------------------------------

			//  ITurboGameMotionEffects Methods ------------------------------------------------------------------------
			virtual void ProcessMotionEffects(
				NavigationInfo* navInfo,
				std::shared_ptr<ITurboSceneObject> sceneObject,
				bool isPlayer);

		private:
			const double cMoveAcceleration = 2.0f;
			const double cRotateAcceleration = 45.0f;
			const double cFrictionFactor = 2.0f;
			const double cHoverFrequency = 2.0f;
			const double cHoverMagnitude = 0.05f;
			const double cGravityFactor = 0.0f;
			const double cSelfRightingSpeed = 0.0f;

			std::shared_ptr<ITurboGameMotionEffects> _baseMotionEffects;
		};
	}
}