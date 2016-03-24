//  ============================================================================
//  SpaceLabyrinth.cpp
//  ============================================================================

#include "pch.h"

#include "SpaceLabyrinth.h"
#include "SpaceLabyrinthOriginalLevel.h"
#include "OriginalMazeFactory.h"

//============  Constructors and Destructors  --------------------------------------------------------------------------

SpaceLabyrinth::SpaceLabyrinth(ISpaceLabyrinthPlatform *platform)
{
	_platform = platform;
}

SpaceLabyrinth::~SpaceLabyrinth()
{
	delete _level;
}

//----------------------------------------------------------------------------------------------------------------------
//============  IProgram Methods  --------------------------------------------------------------------------------------

int SpaceLabyrinth::Initialize()
{
	_level = new SpaceLabyrinthOriginalLevel(_platform);
	return _level->Initialize();
}

void SpaceLabyrinth::SetPlatformResources(IPlatformResources *platformResources)
{
	//  Just pass through.
	_platform->SetPlatformResources(platformResources);
}

int SpaceLabyrinth::Resize(int width, int height)
{
	//  Just pass through.
	return _platform->Resize(width, height);
}

int SpaceLabyrinth::Update()
{
	NavInfo navInfo;
	_platform->GetNavigationInfo(&navInfo);

	if (navInfo.Restart)
	{
		navInfo.Restart = 0;

		delete _level;
		_level = new SpaceLabyrinthOriginalLevel(_platform);
		return _level->Initialize();
	}

	return _level->Update();
}

int SpaceLabyrinth::Render()
{
	return _level->Render();
}

int SpaceLabyrinth::SaveState()
{
	//_platform->SaveState(_state);
	return 1;
}

int SpaceLabyrinth::Finalize()
{
	return _level->Finalize();
}

//----------------------------------------------------------------------------------------------------------------------
