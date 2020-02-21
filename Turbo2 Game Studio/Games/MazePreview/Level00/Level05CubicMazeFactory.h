
#pragma once

#include <ICubicMazeFactory.h>

class Level05CubicMazeFactory: public ICubicMazeFactory
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	Level05CubicMazeFactory(){}
	virtual ~Level05CubicMazeFactory(){}

	//	ICubicMazeFactory Methods --------------------------------------------------------------------------------------
	virtual std::shared_ptr<CubicMaze> MakeMaze(int width, int height, int depth);
	virtual void FreeMaze(std::shared_ptr<CubicMaze> cubicMaze) {}
};

