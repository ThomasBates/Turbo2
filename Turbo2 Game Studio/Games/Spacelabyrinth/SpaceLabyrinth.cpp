//  ============================================================================
//  SpaceLabyrinth.cpp
//  ============================================================================

#include "pch.h"

#include "SpaceLabyrinth.h"
#include "SpaceLabyrinthOriginalLevel.h"

//============  Constructors and Destructors  --------------------------------------------------------------------------

SpaceLabyrinth::SpaceLabyrinth(std::shared_ptr<ITurboApplicationPlatform> platform)
{
	_platform = platform;
}

//----------------------------------------------------------------------------------------------------------------------
//============  IProgram Methods  --------------------------------------------------------------------------------------

int SpaceLabyrinth::Initialize()
{
	_level = std::unique_ptr<IGameLevel>(new SpaceLabyrinthOriginalLevel(_platform));
	return _level->Initialize();
}

void SpaceLabyrinth::SetPlatformResources(std::shared_ptr<IPlatformResources> platformResources)
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
	_platform->SetTimeStampForFrame();
	NavigationInfo navInfo = _platform->GetNavigationInfo();

	if (navInfo.Restart)
	{
		navInfo.Restart = false;

		_level = std::unique_ptr<IGameLevel>(new SpaceLabyrinthOriginalLevel(_platform));
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
