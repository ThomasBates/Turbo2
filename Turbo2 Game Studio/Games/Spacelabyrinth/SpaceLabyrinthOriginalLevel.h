
#pragma once

#include "pch.h"

#include "IProgram.h"
#include "IGameLevel.h"
#include "ITurboSceneBuilder.h"

class SpaceLabyrinthOriginalLevel : public IGameLevel
{
private:
	std::unique_ptr<ITurboSceneBuilder>			_sceneBuilder;
	std::shared_ptr<ITurboScene>				_scene;
	std::shared_ptr<ITurboSceneObject>		    _player;

	int		_pointer;
	int		_pointerX;
	int		_pointerY;

	//  Local Support Methods
	Vector3D	GetSpawnPoint();
	//void	RenderStaticScene();
	//void	UpdateDynamicSceneObjects();
	//void	RenderDynamicSceneObjects();
	void	ProcessObjectInteractions(NavigationInfo navInfo);

public:
	//  Constructors and Destructors  ----------------------------------------------------------------------------------
	SpaceLabyrinthOriginalLevel();
	~SpaceLabyrinthOriginalLevel();

	//	IGameLevel Properties  ---------------------------------------------------------------------------------------------
	virtual std::shared_ptr<IApplicationState> State();
	virtual void State(std::shared_ptr<IApplicationState> state);

	virtual std::shared_ptr<ITurboScene> Scene() { return _scene; }

	//  IGameLevel Methods  --------------------------------------------------------------------------------------------
	virtual void		Initialize();
	virtual void		Finalize();

	virtual void		Update(NavigationInfo navInfo);
	//virtual int		Render();
	//virtual void		SaveState();

};

