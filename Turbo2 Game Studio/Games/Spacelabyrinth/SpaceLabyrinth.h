
#pragma once

#include "IProgram.h"
#include "ISpaceLabyrinthPlatform.h"
#include "IMazeFactory.h"
#include "Camera.h"

class SpaceLabyrinth: public IProgram
{
private:
	ISpaceLabyrinthPlatform *_factory;
	IMazeFactory *_mazeFactory;

	IMaze *_maze;
	int		_pointer;
	int		_pointerX;
	int		_pointerY;

	Camera	_camera;

public:
	//  Constructors and Destructors
	SpaceLabyrinth(ISpaceLabyrinthPlatform *factory);
	~SpaceLabyrinth();

	//  IProgram Methods
	virtual int Initialize();
	virtual int Resize(int width, int height);
	virtual int Update();
	virtual int	Draw();
	virtual int Finalize();

protected:
	//  Local Support Methods
	virtual int NavigateMaze();
	virtual int CheckForBounce(Vector newPosition, const MazeObject *mazeObject);
	virtual int DrawMaze();
};

