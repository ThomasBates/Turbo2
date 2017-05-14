
#pragma once

#include "pch.h"

#include "IGameLevel.h"

class SpaceLabyrinth: public IGameLevel
{
public:
	//  Constructors and Destructors  ----------------------------------------------------------------------------------
	SpaceLabyrinth();

	//  IGameLevel Properties  -----------------------------------------------------------------------------------------
	virtual std::string Title() { return "Space Labyrinth"; }

	virtual std::shared_ptr<IApplicationState> State();
	virtual void State(std::shared_ptr<IApplicationState> state);

	virtual std::shared_ptr<ITurboScene> Scene();

	virtual bool SceneChanged() { return _sceneChanged; }

	//  IGameLevel Methods  --------------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update(NavigationInfo navInfo);

private:
	std::shared_ptr<IGameLevel> _level = nullptr;
	bool _sceneChanged = false;

};

