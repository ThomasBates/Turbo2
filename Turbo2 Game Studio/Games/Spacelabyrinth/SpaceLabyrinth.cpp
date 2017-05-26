//  ============================================================================
//  SpaceLabyrinth.cpp
//  ============================================================================

#include "pch.h"

#include "SpaceLabyrinth.h"
#include "IntroLevel.h"
#include "OriginalLevel.h"

//  Constructors and Destructors ---------------------------------------------------------------------------------------

SpaceLabyrinth::SpaceLabyrinth()
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> SpaceLabyrinth::State()
{
	if (_level == nullptr)
	{
		return nullptr;
	}

	std::shared_ptr<ITurboGameState> state = _level->State();
	state->SaveString("ProgramInfo", "project info");
	return state;
}

void SpaceLabyrinth::State(std::shared_ptr<ITurboGameState> state)
{
	state->LoadString("ProgramInfo");
	_level->State(state);
}

std::shared_ptr<ITurboScene> SpaceLabyrinth::Scene()
{
	return _level->Scene();
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

		_level = std::unique_ptr<ITurboGameLevel>(new IntroLevel());
		_level->Initialize();
		_sceneChanged = true;
	}

	_level->Update(navInfo);
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
