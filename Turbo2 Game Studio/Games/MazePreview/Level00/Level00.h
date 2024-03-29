
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
#include <MazePreviewGameState.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

class Level00 : public ITurboGameLevel
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	Level00(
		std::shared_ptr<ITurboDebug> debug,
		std::shared_ptr<MazePreviewGameState> gameState,
		std::shared_ptr<ITurboSceneObject> player);
	virtual ~Level00(){}

	//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return _subLevel == nullptr ? "Mezzanine" : _subLevel->Title(); }

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

	//	Event Handlers -------------------------------------------------------------------------------------------------
	void GameOptionsOnValueChanged(void *sender, std::shared_ptr<TurboConfigValueChangedEventArgs> args);

private:
	std::shared_ptr<ITurboDebug>			_debug;
	std::shared_ptr<MazePreviewGameState> 	_gameState;
	std::shared_ptr<ITurboSceneObject>		_player;
	std::shared_ptr<CubicMaze>				_maze;
	std::shared_ptr<ITurboScene>			_scene;
	std::shared_ptr<ITurboGameLevel>		_subLevel;
	int										_subLevelIndex = 0;
	TurboGameLevelState						_levelState = TurboGameLevelState::Initializing;
	bool									_sceneChanged = true;
	int 									_actionTriggered = false;
	bool 									_actionStillActive = false;

	std::shared_ptr<ICubicMazeFactory>				_mazeFactory;
	std::shared_ptr<ICubicMazeSceneBuilder>			_sceneBuilder;
	std::shared_ptr<ITurboGameMotionEffects>		_motionEffects;
	std::shared_ptr<ICubicMazeObjectInteractions>	_objectInteractions;
	std::shared_ptr<Level00Helper>					_helper;
	std::shared_ptr<MazeOptions> 					_mazeOptions;

	bool 											_gameOptionsChanged = true;
	std::string                      				_signage = "";

	//	Local Methods --------------------------------------------------------------------------------------------------
	void ValidateGameOptions();
	void UpdateMazeOptions(int currentRound);
	void BuildScene();
};

