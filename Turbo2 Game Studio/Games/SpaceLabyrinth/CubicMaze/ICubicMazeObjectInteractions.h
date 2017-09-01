
#pragma once

#include <pch.h>

#include <ITurboSceneObject.h>
#include <CubicMaze.h>

using namespace Turbo::Scene;

class ICubicMazeObjectInteractions
{
public:
	//	ICubicMazeObjectInteractions Methods ---------------------------------------------------------------------------
	virtual void ProcessObjectInteractions(
		NavigationInfo navInfo,
		std::shared_ptr<CubicMaze> maze,
		std::shared_ptr<ITurboSceneObject> sceneObject,
		bool isPlayer,
		int *pPortalIndex) = 0;
};

