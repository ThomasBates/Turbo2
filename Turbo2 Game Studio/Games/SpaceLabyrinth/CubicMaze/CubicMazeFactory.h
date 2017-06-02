
#pragma once

#include <ICubicMazeFactory.h>

enum       { RIGHT, DOWN, FRONT, LEFT, UP, BACK };

class CubicMazeFactory: public ICubicMazeFactory
{
public:
	//	ICubicMazeFactory Methods --------------------------------------------------------------------------------------
	virtual std::shared_ptr<CubicMaze> MakeMaze(int width, int height, int depth);
	virtual void FreeMaze(std::shared_ptr<CubicMaze> cubicMaze);

private:
	//	Local Support Methods ------------------------------------------------------------------------------------------
	bool Move(CubicMazeDirection mask, std::shared_ptr<CubicMaze> cubicMaze, CubicMazeLocation size, CubicMazeLocation *p, int *count);
	void Relocate(std::shared_ptr<CubicMaze> cubicMaze, CubicMazeLocation size, CubicMazeLocation *p);

	//void AllocateMazeArray(CubicMazeCellArray3D *mazeArray, CubicMazeLocation size);
	//void DeallocateMazeArray(CubicMazeCellArray3D *mazeArray, CubicMazeLocation size);
	//void ClearMazeArray(CubicMazeCellArray3D mazeArray, CubicMazeLocation size);
};

