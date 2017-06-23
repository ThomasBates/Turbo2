
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGame.h>
#include <ITurboGameLevel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

class SpaceLabyrinth: public ITurboGame
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	SpaceLabyrinth(std::shared_ptr<ITurboDebug> debug);

	//  ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "Space Labyrinth"; }

	virtual std::shared_ptr<ITurboGameState> State();
	virtual void State(std::shared_ptr<ITurboGameState> state);

	virtual std::shared_ptr<ITurboScene> Scene();
	virtual std::shared_ptr<ITurboSceneObject> Player();

	virtual bool SceneChanged() { return _sceneChanged; }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update(NavigationInfo navInfo);

private:
	std::shared_ptr<ITurboDebug> _debug;
	std::shared_ptr<ITurboGameLevel> _level = nullptr;
	bool _sceneChanged = false;

};

