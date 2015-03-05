
#pragma once

#include "IMaze.h"

class OriginalMaze: public IMaze
{
private:
	Array3D		_maze;
	location	_size;
	location	_entrance;
	location	_exit;
	loclist		_solution;

public:
	//  Properties
	virtual Array3D GetMaze() { return _maze; }
	virtual void SetMaze(Array3D maze) { _maze = maze; }

	virtual location GetSize() { return _size; }
	virtual void SetSize(location size) { _size = size; }

	virtual location GetEntrance() { return _entrance; }
	virtual void SetEntrance(location entrance) { _entrance = entrance; }

	virtual location GetExit() { return _exit; }
	virtual void SetExit(location exit) { _exit = exit; }
	
	virtual loclist GetSolution() { return _solution; }
	virtual void SetSolution(loclist solution) { _solution = solution; }
};