
#pragma once

#include "IMaze.h"

class IMazeFactory
{
public:
	virtual IMaze *MakeMaze(int width, int height, int depth) = 0;
	virtual void FreeMaze(IMaze *maze) = 0;
};