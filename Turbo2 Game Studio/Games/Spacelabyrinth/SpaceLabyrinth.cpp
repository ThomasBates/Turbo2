//  ============================================================================
//  SpaceLabyrinth.cpp
//  ============================================================================

#include <math.h>
#include <stdlib.h>

#include "SpaceLabyrinth.h"
#include "OriginalMazeFactory.h"

//============  Constructors and Destructors  --------------------------------------------------------------------------

SpaceLabyrinth::SpaceLabyrinth(ISpaceLabyrinthPlatform *platform)
{
	_platform = platform;
	
	_mazeFactory = new OriginalMazeFactory;	
	_maze = 0;
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
	int result = _platform->Initialize(&_camera);
	
//	_camera.Move(2,-2,-2);

	return result;
}

int SpaceLabyrinth::Resize(int width, int height)
{
	return _platform->Resize(width, height);
}

int SpaceLabyrinth::Update()
{
	_platform->BeginUpdate();

	NavigateMaze();

	_platform->EndUpdate();

	return 1;
}

int SpaceLabyrinth::Draw()
{
	_platform->BeginDraw();

	DrawMaze();

	_platform->EndDraw();

	return 1;
}

int SpaceLabyrinth::Finalize()
{
	return _platform->Finalize();
}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods  ---------------------------------------------------------------------------------

int SpaceLabyrinth::NavigateMaze()
{
	NavInfo navInfo;

	_platform->GetNavigationInfo(&navInfo);

	if (navInfo.Restart || _maze == 0)
	{
		_maze = _mazeFactory->MakeMaze(3,3,3);
		_camera.Reset();
		_camera.Move(2,-2,-2);
		
		navInfo.Restart = 0;
		return 1;
	}

	float deltaTime = _platform->GetDeltaTime();
	float time      = _platform->GetTime();

	float moveAccelleration = 1.0;
	float moveSpeed = moveAccelleration * deltaTime;

	if (!(navInfo.MoveLeft  ||
		  navInfo.MoveRight ||
		  navInfo.MoveDown  ||
		  navInfo.MoveUp    ||
		  navInfo.MoveFore  ||
		  navInfo.MoveBack))
	{
		//  air friction decay
		_camera.Velocity -= _camera.Velocity * 1.0 * deltaTime;
		
		//  hover
		_camera.Velocity += _camera.Up * cos(time*2) * 0.05 * deltaTime;

		//  gravity
		//_camera.Velocity.Y -= deltaTime;
	}

	
	if (navInfo.MoveLeft)	_camera.Velocity -= _camera.Right * moveSpeed;
	if (navInfo.MoveRight)	_camera.Velocity += _camera.Right * moveSpeed;
	if (navInfo.MoveDown)	_camera.Velocity -= _camera.Up    * moveSpeed;
	if (navInfo.MoveUp)		_camera.Velocity += _camera.Up    * moveSpeed;
	if (navInfo.MoveFore)	_camera.Velocity -= _camera.Back  * moveSpeed;
	if (navInfo.MoveBack)	_camera.Velocity += _camera.Back  * moveSpeed;
	/*
	if (navInfo.MoveLeft)	_camera.Velocity = -_camera.Right * moveAccelleration;
	if (navInfo.MoveRight)	_camera.Velocity =  _camera.Right * moveAccelleration;
	if (navInfo.MoveDown)	_camera.Velocity = -_camera.Up    * moveAccelleration;
	if (navInfo.MoveUp)		_camera.Velocity =  _camera.Up    * moveAccelleration;
	if (navInfo.MoveFore)	_camera.Velocity = -_camera.Back  * moveAccelleration;
	if (navInfo.MoveBack)	_camera.Velocity =  _camera.Back  * moveAccelleration;
	*/

	float rotateAccelleration = 45.0;
	float rotateSpeed = rotateAccelleration * deltaTime;

	if (!(navInfo.Pointer   ||
		  navInfo.PitchFore ||
		  navInfo.PitchBack ||
		  navInfo.YawRight  ||
		  navInfo.YawLeft   ||
		  navInfo.RollLeft  ||
		  navInfo.RollRight))
	{
		_camera.AngularVelocity -= _camera.AngularVelocity * 1.0 * deltaTime;

		//  self-righting
		//_camera.AngularVelocity.Z -= _camera.Right.Y * 30 * deltaTime;
		//_camera.AngularVelocity.X -= _camera.Back.Y * 30 * deltaTime;
	}

	if (navInfo.Pointer && _pointer)
	{
		int dx	= navInfo.PointerX - _pointerX;
		int dy	= navInfo.PointerY - _pointerY;
		
		_camera.AngularVelocity.X = dy / deltaTime;
		_camera.AngularVelocity.Y =-dx / deltaTime;
	}
	
	_pointer	= navInfo.Pointer;
	_pointerX	= navInfo.PointerX;
	_pointerY	= navInfo.PointerY;

	
	if (navInfo.PitchFore)	_camera.AngularVelocity.X -= rotateSpeed;
	if (navInfo.PitchBack)	_camera.AngularVelocity.X += rotateSpeed;
	if (navInfo.YawRight)	_camera.AngularVelocity.Y -= rotateSpeed;
	if (navInfo.YawLeft)	_camera.AngularVelocity.Y += rotateSpeed;
	if (navInfo.RollLeft)	_camera.AngularVelocity.Z -= rotateSpeed;
	if (navInfo.RollRight)	_camera.AngularVelocity.Z += rotateSpeed;
	/*
	if (navInfo.PitchFore)	_camera.AngularVelocity.X = -rotateAccelleration;
	if (navInfo.PitchBack)	_camera.AngularVelocity.X =  rotateAccelleration;
	if (navInfo.YawRight)	_camera.AngularVelocity.Y = -rotateAccelleration;
	if (navInfo.YawLeft)	_camera.AngularVelocity.Y =  rotateAccelleration;
	if (navInfo.RollLeft)	_camera.AngularVelocity.Z = -rotateAccelleration;
	if (navInfo.RollRight)	_camera.AngularVelocity.Z =  rotateAccelleration;
	*/
	//  Check for collisions and change velocity accordingly.

	Vector newPosition = _camera.Position + _camera.Velocity * deltaTime;

	location size = _maze->GetSize();
	int numCells = (size.w+1) * (size.h+1) * (size.d+1);

	MazeObject *corners = _maze->GetCorners();
	for (int i=0; i<numCells; i++)
	{
		MazeObject *corner = &(corners[i]);
		if (corner->Active)
		{
			if (CheckForBounce(newPosition, corner))
				newPosition = _camera.Position + _camera.Velocity * deltaTime;
		}
	}

	MazeObject *edges = _maze->GetEdges();
	for (int i=0; i<numCells*3; i++)
	{
		MazeObject *edge = &(edges[i]);
		if (edge->Active)
		{
			if (CheckForBounce(newPosition, edge))
				newPosition = _camera.Position + _camera.Velocity * deltaTime;
		}
	}

	MazeObject *walls = _maze->GetWalls();
	for (int i=0; i<numCells*3; i++)
	{
		MazeObject *wall = &(walls[i]);
		if (wall->Active)
		{
			if (CheckForBounce(newPosition, wall))
				newPosition = _camera.Position + _camera.Velocity * deltaTime;
		}
	}

	_camera.Move(_camera.Velocity * deltaTime);
	_camera.Rotate(_camera.AngularVelocity * deltaTime);
	
	return 1;
}

int SpaceLabyrinth::DrawMaze()
{
	location size = _maze->GetSize();
	int numCells = (size.w+1) * (size.h+1) * (size.d+1);

	MazeObject *corners = _maze->GetCorners();
	for (int i=0; i<numCells; i++)
	{
		MazeObject *corner = &(corners[i]);
		if (corner->Active)
			_platform->DrawCorner(corner);
	}

	MazeObject *edges = _maze->GetEdges();
	for (int i=0; i<numCells*3; i++)
	{
		MazeObject *edge = &(edges[i]);
		if (edge->Active)
			_platform->DrawEdge(edge);
	}

	MazeObject *walls = _maze->GetWalls();
	for (int i=0; i<numCells*3; i++)
	{
		MazeObject *wall = &(walls[i]);
		if (wall->Active)
			_platform->DrawWall(wall);
	}

	return 1;							// Everything Went OK
}

int SpaceLabyrinth::CheckForBounce(Vector newPosition, const MazeObject *mazeObject)
{
	const float buffer = 0.25;
	const float bounciness = 0.25;

	if (newPosition.X >= mazeObject->Left - buffer &&
		newPosition.X <= mazeObject->Right + buffer &&
		newPosition.Y >= mazeObject->Bottom - buffer &&
		newPosition.Y <= mazeObject->Top + buffer &&
		newPosition.Z >= mazeObject->Front - buffer &&
		newPosition.Z <= mazeObject->Back + buffer)
	{
		if (_camera.Position.X <= mazeObject->Left - buffer)
			_camera.Velocity.X = -_camera.Velocity.X * bounciness;

		if (_camera.Position.X >= mazeObject->Right + buffer)
			_camera.Velocity.X = -_camera.Velocity.X * bounciness;

		if (_camera.Position.Y <= mazeObject->Bottom - buffer)
			_camera.Velocity.Y = -_camera.Velocity.Y * bounciness;

		if (_camera.Position.Y >= mazeObject->Top + buffer)
			_camera.Velocity.Y = -_camera.Velocity.Y * bounciness;

		if (_camera.Position.Z <= mazeObject->Front - buffer)
			_camera.Velocity.Z = -_camera.Velocity.Z * bounciness;

		if (_camera.Position.Z >= mazeObject->Back + buffer)
			_camera.Velocity.Z = -_camera.Velocity.Z * bounciness;

		return 1;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
