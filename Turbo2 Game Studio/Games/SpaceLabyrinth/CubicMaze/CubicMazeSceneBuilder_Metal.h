
#pragma once

#include <CubicMaze.h>
#include <ICubicMazeSceneBuilder.h>

class CubicMazeSceneBuilder_Metal : public ICubicMazeSceneBuilder
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	CubicMazeSceneBuilder_Metal();

	//  ICubicMazeSceneBuilder Methods ---------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze> cubicMaze);

private:
	std::shared_ptr<ICubicMazeSceneBuilder> _baseSceneBuilder;
};
