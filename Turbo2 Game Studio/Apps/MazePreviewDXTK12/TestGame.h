
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGame.h>
#include <ITurboGameLevel.h>
#include <Level00Types.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

class TestGame: public ITurboGame
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	TestGame(std::shared_ptr<ITurboDebug> debug);
	void ComposeTestScene();

	//  ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "Test Game"; }

	virtual std::shared_ptr<ITurboGameState> GameState();
	virtual void GameState(std::shared_ptr<ITurboGameState> gameState);

	virtual TurboGameLevelState LevelState() { return _levelState; }
	virtual void LevelState(TurboGameLevelState levelState) { _levelState = levelState; }

	virtual std::shared_ptr<ITurboScene> Scene();
	virtual std::shared_ptr<ITurboSceneObject> Player();

	virtual bool SceneChanged() { return _sceneChanged; }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update(NavigationInfo navInfo);

private:
	std::shared_ptr<ITurboDebug>		_debug;
	std::shared_ptr<ITurboScene>		_scene;

	//std::shared_ptr<ITurboSceneObject>	_player;
	//std::shared_ptr<ITurboGameLevel>	_level = nullptr;
	//std::shared_ptr<ITurboGameState>	_gameState;
	bool								_sceneChanged = false;
	TurboGameLevelState					_levelState;
	//Level00UserOptions					_userOptions = { false, true };

	std::shared_ptr<ITurboSceneObject>	_rotatingSceneObject;
};

