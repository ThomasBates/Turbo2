
#pragma once

#include <pch.h>
#include <Level01MotionEffects.h>
#include <TurboGameMotionEffects.h>

using namespace Turbo::Game;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

Level01MotionEffects::Level01MotionEffects()
{
	_baseMotionEffects = std::shared_ptr<ITurboGameMotionEffects>(new TurboGameMotionEffects(
		cMoveAccelleration,
		cRotateAccelleration,
		cFrictionFactor,
		cHoverFrequency,
		cHoverMagnitude,
		cGravityFactor,
		cSelfRightingSpeed));
}

//  ITurboGameState Properties -----------------------------------------------------------------------------------------

//  ITurboGameState Methods --------------------------------------------------------------------------------------------

void Level01MotionEffects::ProcessMotionEffects(NavigationInfo navInfo, std::shared_ptr<ITurboSceneObject> sceneObject, bool isPlayer)
{
	_baseMotionEffects->ProcessMotionEffects(navInfo, sceneObject, isPlayer);

	//  Limit motion to horizontal plane for this level
	TurboVector3D velocity = sceneObject->Placement()->Velocity();
	velocity.Y = 0;
	sceneObject->Placement()->Velocity(velocity);

}

