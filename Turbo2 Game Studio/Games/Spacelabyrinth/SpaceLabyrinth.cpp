//  ============================================================================
//  SpaceLabyrinth.cpp
//  ============================================================================
#include <stdlib.h>

#include "SpaceLabyrinth.h"
#include "OriginalMazeFactory.h"

//  Constructors and Destructors  ==============================================

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

//  IProgram Methods  ==========================================================

int SpaceLabyrinth::Initialize()
{
	return _factory->Initialize();
}

int SpaceLabyrinth::Update()
{
/*
	// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
	if (_active)						// Program Active?
	{

		if (!_program->Update())		// Draw The Scene
		{
			_done = TRUE;
			return FALSE;
		}

	}
*/
	_factory->BeginUpdate();

	DrawMaze(_maze);

	_factory->EndUpdate();

	return 1;
}

int SpaceLabyrinth::Resize(int width, int height)
{
	return _factory->Resize(width, height);
}

int SpaceLabyrinth::Reset()
{
	return _factory->Reset();
}

int SpaceLabyrinth::Finalize()
{
	return _factory->Finalize();
}

//  Local Support Methods  =====================================================

int SpaceLabyrinth::DrawMaze(IMaze *maze)
{
	_factory->DrawWall(0,0,0,1,1,1);

	/*
	Array3D data = maze->GetMaze();
	location size = maze->GetSize();
	

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
	*/

	return 1;							// Everything Went OK
}

int SpaceLabyrinth::DrawRightWall(int w, int h, int d)
{
	_factory->DrawWall(0,0,0,1,1,1);
	return 1;							// Everything Went OK
}

int SpaceLabyrinth::DrawBottomWall(int w, int h, int d)
{
	_factory->DrawWall(0,0,0,1,1,1);
	return 1;							// Everything Went OK
}

int SpaceLabyrinth::DrawFrontWall(int w, int h, int d)
{
	_factory->DrawWall(0,0,0,1,1,1);
	return 1;							// Everything Went OK
}

