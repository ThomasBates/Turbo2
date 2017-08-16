
#pragma once

#include <pch.h>
#include <Level03MotionEffects.h>
#include <TurboGameMotionEffects.h>

using namespace Turbo::Game;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

Level03MotionEffects::Level03MotionEffects()
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

void Level03MotionEffects::ProcessMotionEffects(NavigationInfo navInfo, std::shared_ptr<ITurboSceneObject> sceneObject, bool isPlayer)
{
	_baseMotionEffects->ProcessMotionEffects(navInfo, sceneObject, isPlayer);
}

