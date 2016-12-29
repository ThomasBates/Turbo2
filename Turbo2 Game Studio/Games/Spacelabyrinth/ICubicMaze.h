
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
}	locationlist;

typedef struct
{
	int		Active;
	float	Left, Right;
	float	Top, Bottom;
	float	Back, Front;
}	MazeObject;

class ICubicMaze
{
public:
	//  Properties
	virtual Array3D GetMazeArray() = 0;
	virtual void SetMazeArray(Array3D maze) = 0;

	virtual location GetSize() = 0;
	virtual void SetSize(location size) = 0;

	virtual location GetEntrance() = 0;
	virtual void SetEntrance(location entrance) = 0;

	virtual location GetExit() = 0;
	virtual void SetExit(location exit) = 0;
	
	virtual locationlist GetSolution() = 0;
	virtual void SetSolution(locationlist solution) = 0;

	//virtual MazeObject *GetCorners() = 0;
	//virtual MazeObject *GetEdges() = 0;
	//virtual MazeObject *GetWalls() = 0;

	//  Public Methods
	//virtual void BuildWalls() = 0;
};

