
#pragma once

struct MazeWalls
{
	bool right : 1;
	bool bottom : 1;
	bool front : 1;
	bool used : 1;

	MazeWalls()
	{
		Initialize(false);
	}

	MazeWalls(bool isOn)
	{
		Initialize(isOn);
	}

	void Initialize(bool isOn)
	{
		right = isOn;
		bottom = isOn;
		front = isOn;
		used = isOn;
	}
};

struct Direction
{
	bool left : 1;
	bool up : 1;
	bool back : 1;
	bool right : 1;
	bool down : 1;
	bool front : 1;

	Direction()
	{
		Initialize(false);
	}

	Direction(bool isOn) 
	{
		Initialize(isOn);
	}

	void Initialize(bool isOn)
	{
		left = isOn;
		up = isOn;
		back = isOn;
		right = isOn;
		down = isOn;
		front = isOn;
	}
};


typedef	struct
{
	int w, h, d;
}	location;

typedef	MazeWalls *** Array3D;
typedef	MazeWalls  ** Array2D;
typedef	MazeWalls   * Array1D;

typedef	struct llist
{
	location		posn;
	MazeWalls		mask;
	struct llist	*prev;
	struct llist	*next;
}	locationList;

class CubicMaze
{
private:
	Array3D			_mazeArray;
	location		_size;
	location		_entrance;
	location		_exit;
	locationList	_solution;

public:
	virtual Array3D GetMazeArray() { return _mazeArray; }
	virtual void SetMazeArray(Array3D mazeArray) { _mazeArray = mazeArray; }

	virtual location GetSize() { return _size; }
	virtual void SetSize(location size) { _size = size; }

	virtual location GetEntrance() { return _entrance; }
	virtual void SetEntrance(location entrance) { _entrance = entrance; }

	virtual location GetExit() { return _exit; }
	virtual void SetExit(location exit) { _exit = exit; }

	virtual locationList GetSolution() { return _solution; }
	virtual void SetSolution(locationList solution) { _solution = solution; }
};


#define CELLSIZE ((float)2.0)
#define CELLHALF ((float)1.0)
#define WALLSIZE ((float)0.2)
#define WALLHALF ((float)0.1)
