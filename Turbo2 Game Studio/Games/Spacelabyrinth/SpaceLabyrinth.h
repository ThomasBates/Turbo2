
#pragma once

#include "pch.h"

#include "IProgram.h"
#include "IGameLevel.h"
#include "ISpaceLabyrinthPlatform.h"
//	#include "IMazeFactory.h"
//	#include "Camera.h"

class SpaceLabyrinth: public IProgram
{
private:
	ISpaceLabyrinthPlatform *_platform;
	IGameLevel *_level;
	//	IMazeFactory *_mazeFactory;

	//	IMaze *_maze;
	//	int		_pointer;
	//	int		_pointerX;
	//	int		_pointerY;

	//	Camera	_camera;

	//  Local Support Methods
	//	virtual int NavigateMaze();
	//	virtual int CheckForBounce(Vector newPosition, const MazeObject *mazeObject);
	//	virtual int DrawMaze();

public:
	//  Constructors and Destructors
	SpaceLabyrinth(ISpaceLabyrinthPlatform *platform);
	~SpaceLabyrinth();

	//  IProgram Methods
	virtual LPCWSTR GetTitle() { return TEXT("Space Labyrinth"); }
	virtual int		Initialize();
	virtual void	SetPlatformResources(IPlatformResources *platformResources);
	virtual int		Resize(int width, int height);
	virtual int		Update();
	virtual int		Render();
	virtual int		SaveState();
	virtual int		Finalize();

};

