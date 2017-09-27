
#pragma once

#include <pch.h>

#include <ITurboSceneObject.h>
#include <CubicMaze.h>
#include <TurboGameTypes.h>

using namespace Turbo::Scene;

class ICubicMazeMotionEffects
{
public:
	//	ICubicMazeMotionEffects Properties ---------------------------------------------------------------------

	//	ICubicMazeMotionEffects Methods ------------------------------------------------------------------------
	virtual void ProcessMotionEffects(
		NavigationInfo navInfo,
		std::shared_ptr<CubicMaze> maze,
		std::shared_ptr<ITurboSceneObject> sceneObject,
		bool isPlayer) = 0;
};
