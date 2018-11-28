
#pragma once

#include <pch.h>

#include <ITurboGame.h>
#include <ITurboGameLevel.h>

//using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

class TeapotGame: public ITurboGame
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	//TeapotGame(std::shared_ptr<ITurboDebug> debug);
	TeapotGame();
	virtual ~TeapotGame() {}

	//  ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "More Teapots"; }

	virtual std::shared_ptr<ITurboGameState> GameState();
	virtual void GameState(std::shared_ptr<ITurboGameState> gameState);

	virtual TurboGameLevelState LevelState() { return _levelState; }
	virtual void LevelState(TurboGameLevelState levelState) { _levelState = levelState; }

	virtual std::shared_ptr<ITurboScene> Scene() { return _scene; }
	virtual std::shared_ptr<ITurboSceneObject> Player() { return _player; }

	virtual bool SceneChanged() { return _sceneChanged; }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize() {}
	virtual void Update(NavigationInfo navInfo);

private:
	//	Injected Services ----------------------------------------------------------------------------------------------
	//std::shared_ptr<ITurboDebug>		_debug;

	//	Property Fields ------------------------------------------------------------------------------------------------
	std::shared_ptr<ITurboScene>		_scene;
	std::shared_ptr<ITurboSceneObject>	_player;
	bool								_sceneChanged = false;
	TurboGameLevelState					_levelState;

	//	Local Methods --------------------------------------------------------------------------------------------------
	void BuildScene();
};
