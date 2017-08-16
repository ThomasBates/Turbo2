
#pragma once

#include <pch.h>

#include <ICubicMazeSceneBuilder.h>
#include <ITurboDebug.h>
#include <ITurboGameLevel.h>
#include <ITurboGameMotionEffects.h>
#include <ITurboSceneBuilder.h>

#include <CubicMazeObjectInteractions.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

class Level00 : public ITurboGameLevel
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	Level00(std::shared_ptr<ITurboDebug> debug,
			std::shared_ptr<ITurboSceneObject> player) :
		_debug(debug),
		_player(player)
	{
	}

	//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "Intro Level"; }

	virtual std::shared_ptr<ITurboGameState> GameState();
	virtual void GameState(std::shared_ptr<ITurboGameState> gameState);

	virtual TurboGameLevelState LevelState() { return _levelState; }
	virtual void LevelState(TurboGameLevelState levelState) { _levelState = levelState; }

	virtual std::shared_ptr<ITurboScene> Scene();
	virtual std::shared_ptr<ITurboSceneObject> Player() { return _player; }

	virtual bool SceneChanged() { return _sceneChanged; }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize() {}
	virtual void Update(NavigationInfo navInfo);

private:
	std::shared_ptr<ITurboDebug>		_debug;
	std::shared_ptr<ITurboSceneObject>	_player;
	std::shared_ptr<CubicMaze>			_maze;
	std::shared_ptr<ITurboScene>		_scene;
	std::shared_ptr<ITurboGameLevel>	_subLevel;
	int									_subLevelIndex;
	TurboGameLevelState					_levelState;
	bool								_sceneChanged;

	std::shared_ptr<ICubicMazeSceneBuilder> _sceneBuilder;
	std::shared_ptr<ITurboGameMotionEffects> _motionEffects;
	std::shared_ptr<CubicMazeObjectInteractions> _objectInteractions;

	bool	_level01Unlocked = true;
	bool	_level02Unlocked = false;
	bool	_level03Unlocked = false;
	bool	_level04Unlocked = false;

	int		_pointer;
	int		_pointerX;
	int		_pointerY;

	//	Local Methods --------------------------------------------------------------------------------------------------
	std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze> cubicMaze);
};

