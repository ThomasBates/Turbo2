
#pragma once

#include <pch.h>

#include <ICubicMazeFactory.h>
#include <ICubicMazeSceneBuilder.h>
#include <ITurboDebug.h>
#include <ITurboGameLevel.h>
#include <ITurboGameMotionEffects.h>
#include <ITurboSceneBuilder.h>

#include <CubicMazeObjectInteractions.h>
#include <Level00Helper.h>
#include <Level00Types.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

class Level00 : public ITurboGameLevel
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	Level00(std::shared_ptr<ITurboDebug> debug,
		std::shared_ptr<ITurboSceneObject> player);
	virtual ~Level00(){}

	//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return _subLevel == nullptr ? "Mezzanine" : _subLevel->Title(); }

	virtual std::shared_ptr<ITurboGameState> GameState();
	virtual void GameState(std::shared_ptr<ITurboGameState> gameState);

	virtual TurboGameLevelState LevelState() { return _levelState; }
	virtual void LevelState(TurboGameLevelState levelState) { _levelState = levelState; }

	virtual std::shared_ptr<ITurboScene> Scene();
	virtual std::shared_ptr<ITurboSceneObject> Player() { return _player; }

	virtual std::string GetSignage();

	virtual bool SceneChanged() { return _sceneChanged; }
	virtual int Action();

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize() {}
	virtual void Update(NavigationInfo* navInfo);

private:
	std::shared_ptr<ITurboDebug>		_debug;
	std::shared_ptr<ITurboSceneObject>	_player;
	std::shared_ptr<CubicMaze>			_maze;
	std::shared_ptr<ITurboScene>		_scene;
	std::shared_ptr<ITurboGameLevel>	_subLevel;
	int									_subLevelIndex;
	TurboGameLevelState					_levelState;
	bool								_sceneChanged;
	int 								_actionTriggered;
	bool 								_actionStillActive;

	std::shared_ptr<ICubicMazeFactory>				_mazeFactory;
	std::shared_ptr<ICubicMazeSceneBuilder>			_sceneBuilder;
	std::shared_ptr<ITurboGameMotionEffects>		_motionEffects;
	std::shared_ptr<ICubicMazeObjectInteractions>	_objectInteractions;
	std::shared_ptr<Level00Helper>					_helper;
	std::shared_ptr<ITurboGameState>				_gameState;
	Level00GameOptions								_gameOptions;
	Level00MazeOptions								_mazeOptions;

	std::string                      				_signage;

	//	Local Methods --------------------------------------------------------------------------------------------------
	void UpdateMazeOptions(std::shared_ptr<ICubicMazeSceneBuilder>* sceneBuilder, Level00MazeOptions* mazeOptions);
	void BuildScene(NavigationInfo* navInfo);
};

