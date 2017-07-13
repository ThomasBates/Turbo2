//  ============================================================================
//  SpaceLabyrinth.cpp
//  ============================================================================

#include <pch.h>

#include <SpaceLabyrinth.h>
#include <Level0.h>
#include <OriginalLevel.h>

//	2017-06-07
//	TODO:	GameState.
//	TODO:	Sound effects.
//	TODO:	Object interaction at edges with no walls.
//	TODO:	All lighted first time through. Then dark. Then hazards.
//	TODO:	Lights implemented properly.

//	DONE:
//	2017-06-07:	Player maintains orientation and velocity when going through portals.
//	2017-06-07:	Allow viewing up/down even when navigating flat (e.g. Layer 0 & Layer 0-1).
//	2017-06-14:	Maze Factory for Tiered mazes (e.g. Layer 0-2).
//	2017-06-23:	Lights selectable by layer (lightHack).
//	2017-06-27:	Textures selectable by layer.
//	2017-07-05:	Game Level State (to replace "levelChanged")
//	2017-07-05:	Unlock doors on accomplishments.
//	2017-07-06: Improved navigation in tiered mazes.
//	2017-07-07:	Artwork for Textures.
//	2017-07-10:	Instructions for users.
//	2017-07-11:	Key(s) to unlock exit in level 0-4.
//	2017-07-12:	Hazard(s) in level 0-4

//  Constructors and Destructors ---------------------------------------------------------------------------------------

SpaceLabyrinth::SpaceLabyrinth(std::shared_ptr<ITurboDebug> debug) :
	_debug(debug)
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> SpaceLabyrinth::GameState()
{
	if (_level == nullptr)
	{
		return nullptr;
	}

	std::shared_ptr<ITurboGameState> gameState = _level->GameState();
	gameState->SaveString("ProgramInfo", "project info");
	return gameState;
}

void SpaceLabyrinth::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	if (_level == nullptr)
	{
		return;
	}

	gameState->LoadString("ProgramInfo");
	_level->GameState(gameState);
}

std::shared_ptr<ITurboScene> SpaceLabyrinth::Scene()
{
	if (_level == nullptr)
	{
		return nullptr;
	}

	return _level->Scene();
}

std::shared_ptr<ITurboSceneObject> SpaceLabyrinth::Player()
{
	if (_level == nullptr)
	{
		return nullptr;
	}

	return _level->Player();
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void SpaceLabyrinth::Initialize()
{
	//_level = std::unique_ptr<ITurboGameLevel>(new OriginalLevel());
	//_level->Initialize();	//	Build scene and players
}

void SpaceLabyrinth::Finalize()
{
	if (_level != nullptr)
	{
		_level->Finalize();
		_level = nullptr;
	}
}

void SpaceLabyrinth::Update(NavigationInfo navInfo)
{
	//  Beef this up with multiple levels and state logic.
	_sceneChanged = false;

	if (navInfo.Restart || (_level == nullptr))
	{
		if (_level != nullptr)
		{
			_level->Finalize();
			_level = nullptr;
		}

		//_level = std::unique_ptr<ITurboGameLevel>(new OriginalLevel());
		_level = std::unique_ptr<ITurboGameLevel>(new Level0(_debug));
		_level->Initialize();
		_sceneChanged = true;
	}

	_level->Update(navInfo);

	if (_level->SceneChanged())
	{
		_sceneChanged = true;
	}
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
