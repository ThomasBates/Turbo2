
#pragma once

#include <CubicMaze.h>
#include <ICubicMazeSceneBuilder.h>

class CubicMazeSceneBuilder_Laboratory : public ICubicMazeSceneBuilder
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	CubicMazeSceneBuilder_Laboratory();
	virtual ~CubicMazeSceneBuilder_Laboratory(){}

	//  ICubicMazeSceneBuilder Methods ---------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze> cubicMaze);

private:
	std::shared_ptr<ICubicMazeSceneBuilder> _baseSceneBuilder;
};
