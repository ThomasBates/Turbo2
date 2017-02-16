//  ============================================================================
//  SpaceLabyrinth.cpp
//  ============================================================================

#include "pch.h"

#include "SpaceLabyrinth.h"
#include "SpaceLabyrinthOriginalLevel.h"

//  Constructors and Destructors  --------------------------------------------------------------------------------------

SpaceLabyrinth::SpaceLabyrinth()
{
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  IProgram Properties  -----------------------------------------------------------------------------------------------

std::shared_ptr<IApplicationState> SpaceLabyrinth::State()
{
	std::shared_ptr<IApplicationState> state = _level->State();
	state->SaveString("ProgramInfo", "project info");
	return state;
}

void SpaceLabyrinth::State(std::shared_ptr<IApplicationState> state)
{
	state->LoadString("ProgramInfo");
	_level->State(state);
}

std::shared_ptr<ITurboScene> SpaceLabyrinth::StaticScene()
{
	return _level->Scene();
}

std::shared_ptr<ITurboScene> SpaceLabyrinth::DynamicScene()
{
	return _level->Scene();
}

//  IProgram Properties  -----------------------------------------------------------------------------------------------
//  IProgram Methods  --------------------------------------------------------------------------------------------------

void SpaceLabyrinth::Initialize()
{
	//_level = std::unique_ptr<IGameLevel>(new SpaceLabyrinthOriginalLevel());
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
	_needToRedrawStaticScene = false;

	if ((navInfo.Restart && !_lastRestart) || //  Just get the rising edge of Restart. Don't keep restarting if user keeps their finger on the button.
		(_level == nullptr))
	{
		if (_level != nullptr)
		{
			_level->Finalize();
			_level = nullptr;
		}

		_level = std::unique_ptr<IGameLevel>(new SpaceLabyrinthOriginalLevel());
		_level->Initialize();
		_needToRedrawStaticScene = true;
	}

	_lastRestart = navInfo.Restart;

	_level->Update(navInfo);
}

//  IProgram Methods  --------------------------------------------------------------------------------------------------
