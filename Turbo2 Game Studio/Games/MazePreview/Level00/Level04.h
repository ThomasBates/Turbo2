
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGameLevel.h>
#include <ITurboGameMotionEffects.h>
#include <ITurboSceneBuilder.h>
#include <ICubicMazeFactory.h>

#include <Level00Helper.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

class Level04 : public ITurboGameLevel
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	Level04(std::shared_ptr<ITurboDebug> debug,
		std::shared_ptr<ITurboSceneObject> player,
		std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder,
		std::shared_ptr<MazeOptions> mazeOptions);
	virtual ~Level04(){}

	//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "Maze 4"; }

	virtual TurboGameLevelState LevelState() { return _levelState; }
	virtual void LevelState(TurboGameLevelState levelState) { _levelState = levelState; }

	virtual std::shared_ptr<ITurboScene> Scene() { return _scene; }
	virtual std::shared_ptr<ITurboSceneObject> Player() { return _player; }

	virtual std::string GetSignage(){ return _signage; }

	virtual bool SceneChanged() { return _sceneChanged; }
	virtual int Action() { return 0; }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize() {}
	virtual void Update(NavigationInfo* navInfo);

private:
	//	Injected Services ----------------------------------------------------------------------------------------------
	std::shared_ptr<ITurboDebug>					_debug;
	std::shared_ptr<ICubicMazeFactory>				_mazeFactory;
	std::shared_ptr<ICubicMazeSceneBuilder>			_sceneBuilder;
	std::shared_ptr<ITurboGameMotionEffects>		_motionEffects;
	std::shared_ptr<ICubicMazeObjectInteractions>	_objectInteractions;
	std::shared_ptr<Level00Helper>					_helper;
	std::shared_ptr<MazeOptions>					_mazeOptions;

	//	Property Fields ------------------------------------------------------------------------------------------------
	TurboGameLevelState								_levelState;
	std::shared_ptr<ITurboScene>					_scene;
	std::shared_ptr<ITurboSceneObject>				_player;
	bool											_sceneChanged = false;

	std::shared_ptr<CubicMaze>						_maze;
	std::vector<std::shared_ptr<ITurboSceneObject>>	_keys;
	std::vector<std::shared_ptr<ITurboSceneObject>>	_hazards;

	CubicMazeLocation								_entranceLocation;
	CubicMazeLocation								_exitLocation;

	std::string                      				_signage;

	//	Local Methods --------------------------------------------------------------------------------------------------
	void BuildScene(NavigationInfo* navInfo);
};

