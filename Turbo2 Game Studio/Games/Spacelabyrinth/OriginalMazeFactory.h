
#pragma once

#include "IMazeFactory.h"

enum       { RIGHT, DOWN, FRONT, LEFT, UP, BACK };

class OriginalMazeFactory: public IMazeFactory
{
public:
	virtual IMaze *MakeMaze(int width, int height, int depth);
	virtual void FreeMaze(IMaze *maze);

protected:
	virtual	int Move(char mask, Array3D maze, location size, location *p, int *count);
	virtual	void Relocate(Array3D maze, location size, location *p);
	virtual	void AllocateMaze(Array3D *maze, location size);
	virtual	void DeallocateMaze(Array3D *maze, location size);
	virtual	void ClearMaze(Array3D maze, location size);
};

