
#pragma once

#include <pch.h>

#include <CubicMazeObjectInteractions.h>
#include <ITurboDebug.h>
#include <ITurboGameLevel.h>
#include <ITurboSceneBuilder.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

class Level04 : public ITurboGameLevel
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	Level04(std::shared_ptr<ITurboDebug> debug) :
		_debug(debug),
		_levelState(TurboGameLevelState::Initializing)
	{
	}

	//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "Original Level"; }

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
	std::shared_ptr<ITurboDebug>		_debug;
	std::shared_ptr<CubicMaze>			_maze;
	std::shared_ptr<ITurboScene>		_scene;
	std::shared_ptr<ITurboSceneObject>	_player;
	std::shared_ptr<ITurboSceneObject>	_key;
	std::shared_ptr<ITurboSceneObject>	_hazard;
	TurboGameLevelState					_levelState;

	std::shared_ptr<ICubicMazeSceneBuilder> _sceneBuilder;
	std::shared_ptr<CubicMazeObjectInteractions> _objectInteractions;

	bool	_drawKey = true;
	bool	_drawHazard = true;

	bool	_sceneChanged;
	int		_pointer;
	int		_pointerX;
	int		_pointerY;

	//	Local Methods --------------------------------------------------------------------------------------------------
	std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze> cubicMaze);
};

