
#pragma once

#include "pch.h"

#include "IProgram.h"
#include "IGameLevel.h"
#include "ITurboApplicationPlatform.h"
#include "ITurboSceneBuilder.h"

class SpaceLabyrinthOriginalLevel : public IGameLevel
{
private:
	std::shared_ptr<ITurboApplicationPlatform>	_platform;

	std::unique_ptr<ITurboSceneBuilder>			_sceneBuilder;
	std::shared_ptr<ITurboScene>				_scene;
	std::shared_ptr<ITurboSceneObject>		    _player;

	int		_pointer;
	int		_pointerX;
	int		_pointerY;

	//  Local Support Methods
	Vector3D	GetSpawnPoint();
	void	RenderStaticScene();
	void	UpdateDynamicSceneObjects();
	void	RenderDynamicSceneObjects();
	void	ProcessObjectInteractions();

public:
	//  Constructors and Destructors
	SpaceLabyrinthOriginalLevel(std::shared_ptr<ITurboApplicationPlatform> platform);
	~SpaceLabyrinthOriginalLevel();

	//  IGameLevel Methods
	virtual int		Initialize();
	virtual int		Update();
	virtual int		Render();
	virtual int		SaveState();
	virtual int		Finalize();

};

