
#pragma once

#include "IProgram.h"
#include "ISpaceLabyrinthFactory.h"
#include "IMazeFactory.h"

class SpaceLabyrinth: public IProgram
{
private:
	ISpaceLabyrinthFactory *_factory;
	IMazeFactory *_mazeFactory;

	IMaze *_maze;
	int		_pointer;
	int		_pointerX;
	int		_pointerY;

public:
	//  Constructors and Destructors
	SpaceLabyrinth(ISpaceLabyrinthFactory *factory);
	~SpaceLabyrinth();

	//  IProgram Methods
	virtual int Initialize();
//	virtual int Reset();
	virtual int Resize(int width, int height);
	virtual int Update();
	virtual int Finalize();

protected:
	//  Local Support Methods
	virtual int DrawMaze();
	virtual int DrawRightWall(int w, int h, int d);
	virtual int DrawBottomWall(int w, int h, int d);
	virtual int DrawFrontWall(int w, int h, int d);

	virtual int NavigateMaze();
};