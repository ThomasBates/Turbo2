
#pragma once

#include <pch.h>

#include <ITurboGame.h>
#include <ITurboGameLevel.h>

using namespace Turbo::Game;
using namespace Turbo::Scene;

class SpaceLabyrinth: public ITurboGame
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	SpaceLabyrinth();

	//  ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "Space Labyrinth"; }

	virtual std::shared_ptr<ITurboGameState> State();
	virtual void State(std::shared_ptr<ITurboGameState> state);

	virtual std::shared_ptr<ITurboScene> Scene();

	virtual bool SceneChanged() { return _sceneChanged; }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update(NavigationInfo navInfo);

private:
	std::shared_ptr<ITurboGameLevel> _level = nullptr;
	bool _sceneChanged = false;

};

