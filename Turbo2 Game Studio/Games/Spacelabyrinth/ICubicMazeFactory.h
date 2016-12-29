
#pragma once

#include <CubicMaze.h>

class ICubicMazeFactory
{
public:
	//	ICubicMazeFactory Methods  -------------------------------------------------------------------------------------
	virtual std::shared_ptr<CubicMaze> MakeMaze(int width, int height, int depth) = 0;
	virtual void FreeMaze(std::shared_ptr<CubicMaze> cubicMaze) = 0;
};

