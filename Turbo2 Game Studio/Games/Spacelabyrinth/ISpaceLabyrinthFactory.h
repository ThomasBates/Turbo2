
#pragma once

#include "IMaze.h"

class ISpaceLabyrinthFactory
{
public:
	virtual int		Initialize() = 0;
	virtual int		BeginUpdate() = 0;
	virtual int		EndUpdate() = 0;
	virtual int		Resize(int width, int height) = 0;
	virtual int		Reset() = 0;
	virtual int		Finalize() = 0;

	virtual int		DrawWall(double left, double top, double back, double right, double bottom, double front)  = 0;
};

