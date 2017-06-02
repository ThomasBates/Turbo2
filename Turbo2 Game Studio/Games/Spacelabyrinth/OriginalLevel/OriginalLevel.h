
#pragma once

#include <pch.h>

#include <ITurboGameLevel.h>
#include <ITurboSceneBuilder.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

class OriginalLevel : public ITurboGameLevel
{
private:
	std::shared_ptr<ITurboScene>				_scene;
	std::shared_ptr<ITurboSceneObject>		    _player;

	int		_pointer;
	int		_pointerX;
	int		_pointerY;

	//  Local Support Methods
	TurboVector3D GetSpawnPoint();
	void ProcessObjectInteractions(NavigationInfo navInfo);

public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	OriginalLevel();
	~OriginalLevel();

	//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "Original Level"; }

	virtual std::shared_ptr<ITurboGameState> State();
	virtual void State(std::shared_ptr<ITurboGameState> state);

	virtual std::shared_ptr<ITurboScene> Scene() { return _scene; }

	virtual bool SceneChanged() { return false; }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update(NavigationInfo navInfo);
};

