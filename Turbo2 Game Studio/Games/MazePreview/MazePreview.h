
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGame.h>
#include <ITurboGameLevel.h>
#include <Level00Types.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

class MazePreview : public ITurboGame
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	MazePreview(std::shared_ptr<ITurboDebug> debug);
	virtual ~MazePreview() {}

	//  ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return _level == nullptr ? "Maze Preview" : _level->Title(); }

	virtual std::shared_ptr<ITurboGameState> GameState();
	virtual void GameState(std::shared_ptr<ITurboGameState> gameState);

	virtual TurboGameLevelState LevelState() { return _levelState; }
	virtual void LevelState(TurboGameLevelState levelState) { _levelState = levelState; }

	virtual std::shared_ptr<ITurboScene> Scene();
	virtual std::shared_ptr<ITurboSceneObject> Player();

	virtual bool SceneChanged() { return _sceneChanged; }
	virtual int Action() { return _level == nullptr ? 0 : _level->Action(); }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update(NavigationInfo *navInfo);

private:
	std::shared_ptr<ITurboDebug> _debug;
	std::shared_ptr<ITurboSceneObject> _player;
	std::shared_ptr<ITurboGameLevel> _level = nullptr;
	std::shared_ptr<ITurboGameState> _gameState;
	bool _sceneChanged = false;
	TurboGameLevelState _levelState;
	Level00UserOptions _userOptions = {false, true};
};
