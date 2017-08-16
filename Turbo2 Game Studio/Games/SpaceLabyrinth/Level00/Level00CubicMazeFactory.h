
#pragma once

#include <ICubicMazeFactory.h>

class Level00CubicMazeFactory: public ICubicMazeFactory
{
public:
	//	ICubicMazeFactory Methods --------------------------------------------------------------------------------------
	virtual std::shared_ptr<CubicMaze> MakeMaze(int width, int height, int depth);
	virtual void FreeMaze(std::shared_ptr<CubicMaze> cubicMaze) {}
};

