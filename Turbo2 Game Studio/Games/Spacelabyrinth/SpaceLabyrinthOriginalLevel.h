
#pragma once

#include "pch.h"

#include "IProgram.h"
#include "IGameLevel.h"
#include "ISpaceLabyrinthPlatform.h"
#include "IMazeFactory.h"
#include "Camera.h"

class SpaceLabyrinthOriginalLevel : public IGameLevel
{
private:
	ISpaceLabyrinthPlatform *_platform;
	IMazeFactory *_mazeFactory;

	IMaze *_maze;
	int		_pointer;
	int		_pointerX;
	int		_pointerY;

	Camera	_camera;

	//  Local Support Methods
	virtual int NavigateMaze();
	virtual int CheckForBounce(Vector newPosition, const MazeObject *mazeObject);
	virtual int DrawMaze();

public:
	//  Constructors and Destructors
	SpaceLabyrinthOriginalLevel(ISpaceLabyrinthPlatform *platform);
	~SpaceLabyrinthOriginalLevel();

	//  IProgram Methods
	virtual int		Initialize();
	virtual int		Update();
	virtual int		Render();
	virtual int		SaveState();
	virtual int		Finalize();

};

