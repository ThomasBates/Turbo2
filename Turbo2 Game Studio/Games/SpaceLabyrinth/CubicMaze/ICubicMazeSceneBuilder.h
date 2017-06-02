#pragma once

#include <CubicMaze.h>
#include <ITurboScene.h>

using namespace Turbo::Scene;

class ICubicMazeSceneBuilder
{
public:
	//  ITurboSceneBuilder Methods -------------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze>) = 0;
};
