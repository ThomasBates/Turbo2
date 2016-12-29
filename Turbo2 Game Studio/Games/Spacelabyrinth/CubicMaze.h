
#pragma once

#define		RIGHTBIT	0
#define		BOTTOMBIT	1
#define		FRONTBIT	2
#define		SPACEBIT	6
#define		USEDBIT		7

#define SETBIT(c,b)	((c) = (c) | (1<<(b)))
#define RESETBIT(c,b)	((c) = (c) & (0xff - (1<<(b))))
#define GETBIT(c,b)	((c) & (1<<(b)))

typedef	struct
{
	int w, h, d;
}	location;

typedef	char *** Array3D;
typedef	char  ** Array2D;
typedef	char   * Array1D;

typedef	struct llist
{
	location		posn;
	char			mask;
	struct llist	*prev;
	struct llist	*next;
}	locationList;

typedef struct
{
	int		Active;
	float	Left, Right;
	float	Top, Bottom;
	float	Back, Front;
}	MazeObject;

typedef struct
{
	Array3D			MazeArray;
	location		Size;
	location		Entrance;
	location		Exit;
	locationList	Solution;
}	_CubicMaze;


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
#define OFFSET   ((float)0.0)
#define CELLHALF ((float)1.0)
#define WALLHALF ((float)0.1)
