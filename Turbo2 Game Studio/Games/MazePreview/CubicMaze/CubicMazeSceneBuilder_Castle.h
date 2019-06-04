
#pragma once

#include <CubicMaze.h>
#include <ICubicMazeSceneBuilder.h>

class CubicMazeSceneBuilder_Castle : public ICubicMazeSceneBuilder
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	CubicMazeSceneBuilder_Castle();
	virtual ~CubicMazeSceneBuilder_Castle(){}

	//  ICubicMazeSceneBuilder Methods ---------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze> cubicMaze);

private:
	std::shared_ptr<ICubicMazeSceneBuilder> _baseSceneBuilder;
};
