
#include <pch.h>
#include <CubicMazeMotionEffects_WithoutGravity.h>
#include <TurboGameMotionEffects.h>

using namespace Turbo::Game;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

CubicMazeMotionEffects_WithoutGravity::CubicMazeMotionEffects_WithoutGravity()
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

void CubicMazeMotionEffects_WithoutGravity::ProcessMotionEffects(NavigationInfo* navInfo, std::shared_ptr<ITurboSceneObject> sceneObject, bool isPlayer)
{
	_baseMotionEffects->ProcessMotionEffects(navInfo, sceneObject, isPlayer);
}

