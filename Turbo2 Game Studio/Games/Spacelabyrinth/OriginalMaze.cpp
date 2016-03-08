
#include "pch.h"

#include "OriginalMaze.h"

//============  OriginalMaze  ------------------------------------------------------------------------------------------

//============  Constructors and Destructors  --------------------------------------------------------------------------

OriginalMaze::OriginalMaze()
{
	_corners = 0;
	_edges   = 0;
	_walls   = 0;
}

OriginalMaze::~OriginalMaze()
{
	delete [] _corners;
	delete [] _edges;
	delete [] _walls;
}

//----------------------------------------------------------------------------------------------------------------------
//============  Public Methods  ----------------------------------------------------------------------------------------

void OriginalMaze::BuildWalls()
{
	int numCells = (_size.w+1) * (_size.h+1) * (_size.d+1);
	_corners = new MazeObject[numCells];
	_edges   = new MazeObject[numCells * 3];
	_walls   = new MazeObject[numCells * 3];

	memset(_corners, 0, sizeof(MazeObject) * numCells);
	memset(_edges,   0, sizeof(MazeObject) * numCells * 3);
	memset(_walls,   0, sizeof(MazeObject) * numCells * 3);

	
	for (int w=0; w<=_size.w; w++ )
	for (int h=0; h<=_size.h; h++ )
	for (int d=0; d<=_size.d; d++ )
	{
		BuildCorner(w,h,d);

		if (w>0)	BuildWEdge(w,h,d);
		if (h>0)	BuildHEdge(w,h,d);
		if (d>0)	BuildDEdge(w,h,d);

		char walls = _maze[w][h][d];
		if (GETBIT(walls, RIGHTBIT ) > 0)	BuildRightWall(w,h,d);
		if (GETBIT(walls, BOTTOMBIT) > 0)	BuildBottomWall(w,h,d);
		if (GETBIT(walls, FRONTBIT ) > 0)	BuildFrontWall(w,h,d);
	}

}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Methods  ----------------------------------------------------------------------------------------

int OriginalMaze::BuildCorner(int w, int h, int d)
{
	int index = w + (h + d*(_size.h+1))*(_size.w+1);
	MazeObject *mazeObject = &(_corners[index]);

	mazeObject->Active	= 1;
	mazeObject->Left	= w* CELLSIZE-OFFSET+(CELLHALF-WALLHALF);	//	left
	mazeObject->Top		= h*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	top
	mazeObject->Back	= d*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	back
	mazeObject->Right	= w* CELLSIZE-OFFSET+(CELLHALF+WALLHALF);	//	right
	mazeObject->Bottom	= h*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF);	//	bottom
	mazeObject->Front	= d*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF);	//	front

	return 1;							// Everything Went OK
}

int OriginalMaze::BuildWEdge(int w, int h, int d)
{
	int index = (w + (h + d*(_size.h+1))*(_size.w+1))*3 + 0;
	MazeObject *mazeObject = &(_edges[index]);

	mazeObject->Active	= 1;
	mazeObject->Left	= w* CELLSIZE-OFFSET-(CELLHALF-WALLHALF);	//	left
	mazeObject->Top		= h*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	top
	mazeObject->Back	= d*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	back
	mazeObject->Right	= w* CELLSIZE-OFFSET+(CELLHALF-WALLHALF);	//	right
	mazeObject->Bottom	= h*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF);	//	bottom
	mazeObject->Front	= d*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF);	//	front

	return 1;							// Everything Went OK
}

int OriginalMaze::BuildHEdge(int w, int h, int d)
{
	int index = (w + (h + d*(_size.h+1))*(_size.w+1))*3 + 1;
	MazeObject *mazeObject = &(_edges[index]);

	mazeObject->Active	= 1;
	mazeObject->Left	= w* CELLSIZE-OFFSET+(CELLHALF-WALLHALF);	//	left
	mazeObject->Top		= h*-CELLSIZE+OFFSET+(CELLHALF-WALLHALF);	//	top
	mazeObject->Back	= d*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	back
	mazeObject->Right	= w* CELLSIZE-OFFSET+(CELLHALF+WALLHALF);	//	right
	mazeObject->Bottom	= h*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	bottom
	mazeObject->Front	= d*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF);	//	front

	return 1;							// Everything Went OK
}

int OriginalMaze::BuildDEdge(int w, int h, int d)
{
	int index = (w + (h + d*(_size.h+1))*(_size.w+1))*3 + 2;
	MazeObject *mazeObject = &(_edges[index]);

	mazeObject->Active	= 1;
	mazeObject->Left	= w* CELLSIZE-OFFSET+(CELLHALF-WALLHALF);	//	left
	mazeObject->Top		= h*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	top
	mazeObject->Back	= d*-CELLSIZE+OFFSET+(CELLHALF-WALLHALF);	//	back
	mazeObject->Right	= w* CELLSIZE-OFFSET+(CELLHALF+WALLHALF);	//	right
	mazeObject->Bottom	= h*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF);	//	bottom
	mazeObject->Front	= d*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	front

	return 1;							// Everything Went OK
}

int OriginalMaze::BuildRightWall(int w, int h, int d)
{
	int index = (w + (h + d*(_size.h+1))*(_size.w+1))*3 + 0;
	MazeObject *mazeObject = &(_walls[index]);

	mazeObject->Active	= 1;
	mazeObject->Left	= w* CELLSIZE-OFFSET+(CELLHALF-WALLHALF);	//	left
	mazeObject->Top		= h*-CELLSIZE+OFFSET+(CELLHALF-WALLHALF);	//	top
	mazeObject->Back	= d*-CELLSIZE+OFFSET+(CELLHALF-WALLHALF);	//	back
	mazeObject->Right	= w* CELLSIZE-OFFSET+(CELLHALF+WALLHALF);	//	right
	mazeObject->Bottom	= h*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	bottom
	mazeObject->Front	= d*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	front

	return 1;							// Everything Went OK
}

int OriginalMaze::BuildBottomWall(int w, int h, int d)
{
	int index = (w + (h + d*(_size.h+1))*(_size.w+1))*3 + 1;
	MazeObject *mazeObject = &(_walls[index]);

	mazeObject->Active	= 1;
	mazeObject->Left	= w* CELLSIZE-OFFSET-(CELLHALF-WALLHALF);	//	left
	mazeObject->Top		= h*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	top
	mazeObject->Back	= d*-CELLSIZE+OFFSET+(CELLHALF-WALLHALF);	//	back
	mazeObject->Right	= w* CELLSIZE-OFFSET+(CELLHALF-WALLHALF);	//	right
	mazeObject->Bottom	= h*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF);	//	bottom
	mazeObject->Front	= d*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	front

	return 1;							// Everything Went OK
}

int OriginalMaze::BuildFrontWall(int w, int h, int d)
{
	int index = (w + (h + d*(_size.h+1))*(_size.w+1))*3 + 2;
	MazeObject *mazeObject = &(_walls[index]);

	mazeObject->Active	= 1;
	mazeObject->Left	= w* CELLSIZE-OFFSET-(CELLHALF-WALLHALF);	//	left
	mazeObject->Top		= h*-CELLSIZE+OFFSET+(CELLHALF-WALLHALF);	//	top
	mazeObject->Back	= d*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	back
	mazeObject->Right	= w* CELLSIZE-OFFSET+(CELLHALF-WALLHALF);	//	right
	mazeObject->Bottom	= h*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF);	//	bottom
	mazeObject->Front	= d*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF);	//	front

	return 1;							// Everything Went OK
}

//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

