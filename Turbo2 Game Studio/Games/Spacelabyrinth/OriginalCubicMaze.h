
#pragma once

#include "ICubicMaze.h"

class OriginalCubicMaze: public ICubicMaze
{
private:
	Array3D			_mazeArray;
	location		_size;
	location		_entrance;
	location		_exit;
	locationlist	_solution;

	//MazeObject*		_corners;
	//MazeObject*		_edges;
	//MazeObject*		_walls;

public:
	//OriginalCubicMaze();
	//~OriginalCubicMaze();

	//  Properties
	virtual Array3D GetMazeArray() { return _mazeArray; }
	virtual void SetMazeArray(Array3D mazeArray) { _mazeArray = mazeArray; }

	virtual location GetSize() { return _size; }
	virtual void SetSize(location size) { _size = size; }

	virtual location GetEntrance() { return _entrance; }
	virtual void SetEntrance(location entrance) { _entrance = entrance; }

	virtual location GetExit() { return _exit; }
	virtual void SetExit(location exit) { _exit = exit; }
	
	virtual locationlist GetSolution() { return _solution; }
	virtual void SetSolution(locationlist solution) { _solution = solution; }

	//virtual MazeObject *GetCorners() { return _corners; }
	//virtual MazeObject *GetEdges() { return _edges; }
	//virtual MazeObject *GetWalls() { return _walls; }

	////  IMaze Methods
	//virtual void BuildWalls();

	////  Local Methods
	//virtual int BuildCorner(int w, int h, int d);
	//virtual int BuildWEdge(int w, int h, int d);
	//virtual int BuildHEdge(int w, int h, int d);
	//virtual int BuildDEdge(int w, int h, int d);
	//virtual int BuildRightWall(int w, int h, int d);
	//virtual int BuildBottomWall(int w, int h, int d);
	//virtual int BuildFrontWall(int w, int h, int d);

};

#define CELLSIZE ((float)2.0)
#define OFFSET   ((float)0.0)
#define CELLHALF ((float)1.0)
#define WALLHALF ((float)0.1)
