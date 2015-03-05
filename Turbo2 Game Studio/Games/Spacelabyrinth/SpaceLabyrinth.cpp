//  ============================================================================
//  SpaceLabyrinth.cpp
//  ============================================================================

#include <stdlib.h>

#include "SpaceLabyrinth.h"
#include "OriginalMazeFactory.h"

//============  Constructors and Destructors  --------------------------------------------------------------------------

SpaceLabyrinth::SpaceLabyrinth(ISpaceLabyrinthFactory *factory)
{
	_factory = factory;
	
	_mazeFactory = new OriginalMazeFactory;	
	
	_maze = _mazeFactory->MakeMaze(3,3,3);
}

SpaceLabyrinth::~SpaceLabyrinth()
{
	_mazeFactory->FreeMaze(_maze);
	delete _maze;
	delete _mazeFactory;
}

//----------------------------------------------------------------------------------------------------------------------
//============  IProgram Methods  --------------------------------------------------------------------------------------

int SpaceLabyrinth::Initialize()
{
	return _factory->Initialize();
}

/*
int SpaceLabyrinth::Reset()
{
	return _factory->Reset();
}
*/

int SpaceLabyrinth::Resize(int width, int height)
{
	return _factory->Resize(width, height);
}

int SpaceLabyrinth::Update()
{
	_factory->BeginUpdate();

	DrawMaze();

	NavigateMaze();

	_factory->EndUpdate();

	return 1;
}

int SpaceLabyrinth::Finalize()
{
	return _factory->Finalize();
}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods  ---------------------------------------------------------------------------------

int SpaceLabyrinth::DrawMaze()
{
	location size = _maze->GetSize();
	Array3D data = _maze->GetMaze();
	
	for (int i=0; i<=size.w; i++ )
	for (int j=0; j<=size.h; j++ )
	for (int k=0; k<=size.d; k++ )
	{
		char walls = data[i][j][k];
		
		if (GETBIT(walls, RIGHTBIT ) > 0)
			DrawRightWall(i,j,k);
		if (GETBIT(walls, BOTTOMBIT) > 0)
			DrawBottomWall(i,j,k);
		if (GETBIT(walls, FRONTBIT ) > 0)
			DrawFrontWall(i,j,k);
	}

	return 1;							// Everything Went OK
}

#define CELLSIZE 2.0
#define OFFSET   0.0
#define CELLHALF 1.0
#define WALLHALF 0.1

int SpaceLabyrinth::DrawRightWall(int w, int h, int d)
{
	_factory->DrawWall(	w* CELLSIZE-OFFSET+(CELLHALF-WALLHALF),	//	left
						h*-CELLSIZE+OFFSET+(CELLHALF+WALLHALF),	//	top
						d*-CELLSIZE+OFFSET+(CELLHALF+WALLHALF),	//	back
						w* CELLSIZE-OFFSET+(CELLHALF+WALLHALF),	//	right
						h*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF),	//	bottom
						d*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF));	//	front
	return 1;							// Everything Went OK
}

int SpaceLabyrinth::DrawBottomWall(int w, int h, int d)
{
	_factory->DrawWall(	w* CELLSIZE-OFFSET-(CELLHALF+WALLHALF),	//	left
						h*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF),	//	top
						d*-CELLSIZE+OFFSET+(CELLHALF+WALLHALF),	//	back
						w* CELLSIZE-OFFSET+(CELLHALF+WALLHALF),	//	right
						h*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF),	//	bottom
						d*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF));	//	front
	return 1;							// Everything Went OK
}

int SpaceLabyrinth::DrawFrontWall(int w, int h, int d)
{
	_factory->DrawWall(	w* CELLSIZE-OFFSET-(CELLHALF+WALLHALF),	//	left
						h*-CELLSIZE+OFFSET+(CELLHALF+WALLHALF),	//	top
						d*-CELLSIZE+OFFSET-(CELLHALF-WALLHALF),	//	back
						w* CELLSIZE-OFFSET+(CELLHALF+WALLHALF),	//	right
						h*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF),	//	bottom
						d*-CELLSIZE+OFFSET-(CELLHALF+WALLHALF));	//	front
	return 1;							// Everything Went OK
}


int SpaceLabyrinth::NavigateMaze()
{
	NavInfo navInfo;

	_factory->GetNavigationInfo(&navInfo);

	if (navInfo.Pointer)
	{
		if (_pointer)
		{
			int dx	= navInfo.PointerX - _pointerX;
			int dy	= navInfo.PointerY - _pointerY;
			
			_factory->RotateCamera(dy, dx, 0);
		}
	
		_pointer	= navInfo.Pointer;
		_pointerX	= navInfo.PointerX;
		_pointerY	= navInfo.PointerY;
	}

	float deltaTime = _factory->GetDeltaTime();
	float moveSpeed = 10.0;
	float moveDistance = moveSpeed * deltaTime;
	float rotateSpeed = 10.0;
	float rotateAngle = rotateSpeed * deltaTime;

	if (navInfo.MoveLeft)	{ _factory->MoveCamera(-moveDistance, 0, 0); }
	if (navInfo.MoveRight)	{ _factory->MoveCamera( moveDistance, 0, 0); }
	if (navInfo.MoveDown)	{ _factory->MoveCamera(0, -moveDistance, 0); }
	if (navInfo.MoveUp)		{ _factory->MoveCamera(0,  moveDistance, 0); }
	if (navInfo.MoveFore)	{ _factory->MoveCamera(0, 0, -moveDistance); }
	if (navInfo.MoveBack)	{ _factory->MoveCamera(0, 0,  moveDistance); }
	
	if (navInfo.PitchFore)	{ _factory->RotateCamera(-rotateAngle, 0, 0); }
	if (navInfo.PitchBack)	{ _factory->RotateCamera( rotateAngle, 0, 0); }
	if (navInfo.YawRight)	{ _factory->RotateCamera(0, -rotateAngle, 0); }
	if (navInfo.YawLeft)	{ _factory->RotateCamera(0,  rotateAngle, 0); }
	if (navInfo.RollLeft)	{ _factory->RotateCamera(0, 0, -rotateAngle); }
	if (navInfo.RollRight)	{ _factory->RotateCamera(0, 0,  rotateAngle); }

	return 1;
}

//----------------------------------------------------------------------------------------------------------------------
