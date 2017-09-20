
#pragma once

#include <pch.h>

#include <ITurboSceneObject.h>
#include <CubicMaze.h>

using namespace Turbo::Scene;

class ICubicMazeObjectInteractions
{
public:
	//	ICubicMazeObjectInteractions Methods ---------------------------------------------------------------------------
	virtual void ProcessKeyHazardInteractions(
		NavigationInfo navInfo,
		std::shared_ptr<ITurboSceneObject> sceneObject,
		std::vector<std::shared_ptr<ITurboSceneObject>>* keys,
		std::vector<std::shared_ptr<ITurboSceneObject>>* hazards) = 0;

	virtual void ProcessObjectInteractions(
		NavigationInfo navInfo,
		std::shared_ptr<CubicMaze> maze,
		std::shared_ptr<ITurboSceneObject> sceneObject,
		bool isPlayer,
		int *pPortalIndex) = 0;
};

