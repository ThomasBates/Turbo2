
#pragma once

#include "ICubicMazeFactory.h"

enum       { RIGHT, DOWN, FRONT, LEFT, UP, BACK };

class IntroCubicMazeFactory: public ICubicMazeFactory
{
public:
	//	ICubicMazeFactory Methods --------------------------------------------------------------------------------------
	virtual std::shared_ptr<CubicMaze> MakeMaze(int width, int height, int depth);
	virtual void FreeMaze(std::shared_ptr<CubicMaze> cubicMaze);

private:
	//	Local Support Methods ------------------------------------------------------------------------------------------
	bool Move(Direction mask, Array3D mazeArray, location size, location *p, int *count);
	void Relocate(Array3D mazeArray, location size, location *p);
	void AllocateMazeArray(Array3D *mazeArray, location size);
	void DeallocateMazeArray(Array3D *mazeArray, location size);
	void ClearMazeArray(Array3D mazeArray, location size);
};

