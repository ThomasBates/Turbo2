
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGame.h>
#include <ITurboGameLevel.h>
#include <MazePreviewGameState.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

class MazePreview : public ITurboGame
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	MazePreview(std::shared_ptr<ITurboDebug> debug, std::shared_ptr<MazePreviewGameState> gameState);
	virtual ~MazePreview() {}

	//  ITurboGameLevel Properties -------------------------------------------------------------------------------------
	virtual std::string Title() { return _level == nullptr ? "Maze Preview" : _level->Title(); }

	virtual TurboGameLevelState LevelState() { return _levelState; }
	virtual void LevelState(TurboGameLevelState levelState) { _levelState = levelState; }

	virtual std::shared_ptr<ITurboScene> Scene();
	virtual std::shared_ptr<ITurboSceneObject> Player();

	virtual std::string GetSignage();

	virtual bool SceneChanged() { return _sceneChanged; }
	virtual int Action() { return _level == nullptr ? 0 : _level->Action(); }

	//  ITurboGameLevel Methods ----------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update(NavigationInfo *navInfo);

private:
	std::shared_ptr<ITurboDebug> _debug;
	std::shared_ptr<MazePreviewGameState> _gameState;

	std::shared_ptr<ITurboSceneObject> _player;
	std::shared_ptr<ITurboGameLevel> _level = nullptr;
	bool _sceneChanged = false;
	TurboGameLevelState _levelState = TurboGameLevelState::Initializing;
};
