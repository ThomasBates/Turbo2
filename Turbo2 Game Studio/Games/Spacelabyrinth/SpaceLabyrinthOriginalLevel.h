
#pragma once

#include "pch.h"

#include "IGameLevel.h"
#include "ITurboSceneBuilder.h"

class SpaceLabyrinthOriginalLevel : public IGameLevel
{
private:
	std::unique_ptr<ITurboSceneBuilder>			_sceneBuilder;
	std::shared_ptr<ITurboScene>				_staticScene;
	std::shared_ptr<ITurboScene>				_dynamicScene;
	std::shared_ptr<ITurboSceneObject>		    _player;

	int		_pointer;
	int		_pointerX;
	int		_pointerY;

	//  Local Support Methods
	Vector3D	GetSpawnPoint();
	//void	LoadLevel();
	//void	UpdateDynamicSceneObjects();
	//void	RenderDynamicSceneObjects();
	void	ProcessObjectInteractions(NavigationInfo navInfo);

public:
	//  Constructors and Destructors  ----------------------------------------------------------------------------------
	SpaceLabyrinthOriginalLevel();
	~SpaceLabyrinthOriginalLevel();

	//	IGameLevel Properties  ---------------------------------------------------------------------------------------------
	virtual std::string Title() { return "Original Level"; }

	virtual std::shared_ptr<IApplicationState> State();
	virtual void State(std::shared_ptr<IApplicationState> state);

	virtual std::shared_ptr<ITurboScene> StaticScene() { return _staticScene; }
	virtual std::shared_ptr<ITurboScene> DynamicScene() { return _dynamicScene; }

	virtual bool SceneChanged() { return false; }

	//  IGameLevel Methods  --------------------------------------------------------------------------------------------
	virtual void		Initialize();
	virtual void		Finalize();

	virtual void		Update(NavigationInfo navInfo);
};

