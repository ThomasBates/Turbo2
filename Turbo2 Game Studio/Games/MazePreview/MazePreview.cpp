//  ============================================================================
//  MazePreview.cpp
//  ============================================================================

#include <pch.h>

#include <MazePreview.h>

#include <TurboGameState.h>
#include <Level00.h>
#include <Level00Player.h>

//	2017-06-07
//	TODO:	portal sound only when portalling.
//	TODO:	Numbered Levels
//	TODO:	Sound effect when falling->landing.
//	TODO:	Different wall hit sound with different textures.
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
//	2017-08-16:	Sound effects.
//	2017-08-31:	Game Order (e.g. All lighted first time through. Then keys, Then hazards. Then dark. etc.)
//	2017-09-04:	On gravity levels, keep player upright. (Rotate around level z-axis, not player z-axis.)
//	2017-09-04:	Inverted mouse control.
//	2017-09-05:	Interact with "menu" items (Cross-hairs and select button or something similar.)
//	2017-09-12:	Object interaction at edges with no walls.
//	2017-09-13:	GameState.
//	2017-09-19:	Key & hazard interactions.


/*	Game Order (e.g. All lighted first time through. Then dark. Then hazards. etc.)
	Notes
	-	possible variations from one "level" to the next:
		-	no gravity on all sublevels, and main level.
		-	different textures.
		-	keys on all sublevels.
		-	hazards on all sublevels.
		-	multiple hazards on sublevels.
		-	lights out in sublevels and main level.
		-	still keys, then moving keys.
		-	still hazards, then moving hazards.
		-	larger mazes.

	-	future development
		-	paint
		-	bombs
		-	smart keys
		-	smart hazards
		-	timers
		-	HUD?

	Round 1	- white walls
			- gravity in main level and sublevels 1 and 2, not 3 and 4.
			- no keys.
			- no hazards.
			- lights on.

	Round 2 + castle walls
			+ no gravity
			+ static keys
			- no hazards
			- lights on

	Round 3	- castle walls
			- no gravity
			+ moving keys
			+ static hazards
			- lights on

	Round 4	- castle walls
			- no gravity
			+ moving keys
			+ moving hazards
			+ lights off

	Round 5	- other walls?
			- no gravity
			- moving keys
			- moving hazards
			- lights off
			+ 4x4x4 mazes.

	Round 6	- wall panels to choose options?
*/


//	https://docs.microsoft.com/en-us/windows/uwp/gaming/

//	Terminal 26 - THE POWER FOR GAME DEVELOPERS WITH LOW POLY 3D MODELS AND TEXTURES
//	http://www.terminal26.de/e_index.php

//  Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreview::MazePreview(std::shared_ptr<ITurboDebug> debug, std::shared_ptr<MazePreviewGameState> gameState) :
	_debug(std::move(debug)),
	_gameState(std::move(gameState))
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> MazePreview::Scene()
{
	if (_level == nullptr)
	{
		return nullptr;
	}

	return _level->Scene();
}

std::shared_ptr<ITurboSceneObject> MazePreview::Player()
{
	return _player;
}


std::string MazePreview::GetSignage()
{
	if (_level == nullptr)
	{
		return "Welcome!";
	}

	return _level->GetSignage();
}


//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void MazePreview::Initialize()
{
	_player = std::shared_ptr<ITurboSceneObject>(new Level00Player(_gameState->User()));
}

void MazePreview::Finalize()
{
	if (_level != nullptr)
	{
		_level->Finalize();
		_level = nullptr;
	}
}

void MazePreview::Update(NavigationInfo* navInfo)
{
	//  Beef this up with multiple levels and state logic.
	_sceneChanged = false;

	if (navInfo->Start || (_level == nullptr))
	{
		if (_level != nullptr)
		{
			_level->Finalize();
			_level = nullptr;
		}

		//_userOptions.InvertedMouse = !_userOptions.InvertedMouse;
		_level = std::shared_ptr<ITurboGameLevel>(new Level00(_debug, _gameState, _player));
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
