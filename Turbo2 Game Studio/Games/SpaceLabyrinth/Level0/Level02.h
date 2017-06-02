
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

class Level02 : public ITurboGameLevel
{
private:
	std::shared_ptr<ITurboDebug>		_debug;
	std::shared_ptr<CubicMaze>			_maze;
	std::shared_ptr<ITurboScene>		_scene;
	std::shared_ptr<ITurboSceneObject>	_player;

	std::shared_ptr<CubicMazeObjectInteractions> _objectInteractions;

	bool	_sceneChanged;
	int		_pointer;
	int		_pointerX;
	int		_pointerY;

public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	Level02(std::shared_ptr<ITurboDebug> debug) :
		_debug(debug)
	{}

	//	ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return "Original Level"; }

	virtual std::shared_ptr<ITurboGameState> State();
	virtual void State(std::shared_ptr<ITurboGameState> state);

	virtual std::shared_ptr<ITurboScene> Scene() { return _scene; }

	virtual bool SceneChanged() { return _sceneChanged; }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize() {}
	virtual void Update(NavigationInfo navInfo);
};

