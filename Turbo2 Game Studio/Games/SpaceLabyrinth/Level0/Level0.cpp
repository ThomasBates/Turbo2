
#include <pch.h>

#include <CubicMazeSceneBuilder.h>
#include <Level0.h>
#include <Level01.h>
#include <Level02.h>
#include <Level03.h>
#include <Level04.h>
#include <Level0CubicMazeFactory.h>
#include <Level0Player.h>
#include <TurboGameState.h>
#include <OriginalPlayer.h>
#include <OriginalLevel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level0::State()
{
	std::shared_ptr<ITurboGameState> state = std::shared_ptr<ITurboGameState>(new TurboGameState());
	state->SaveString("LevelInfo", "level info");
	return state;
}

void Level0::State(std::shared_ptr<ITurboGameState> state)
{
	state->LoadString("LevelInfo");
}

std::shared_ptr<ITurboScene> Level0::Scene()
{
	if (_subLevel != nullptr)
	{
		return _subLevel->Scene();
	}

	return _scene;
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level0::Initialize()
{
	//	Create the maze.
	std::shared_ptr<ICubicMazeFactory> mazeFactory = std::shared_ptr<ICubicMazeFactory>(new Level0CubicMazeFactory());
	_maze = mazeFactory->MakeMaze(0, 0, 0);

	//	Build the scene.
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder());
	_scene = sceneBuilder->BuildScene(_maze);

	//  Create the player
	_player = std::shared_ptr<ITurboSceneObject>(new Level0Player());
	_player->Placement()->Reset();
	_player->Placement()->GoTo(8, 0, -6);

	////	set player Placement as camera Placement.
	_scene->CameraPlacement(_player->Placement());

	//  Create NPC's and obstacles ...
	//  ...

	//LoadLevel();

	_objectInteractions = std::shared_ptr<CubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug, _maze, _player));
}

void Level0::Update(NavigationInfo navInfo)
{
	_sceneChanged = false;

	if (_subLevel != nullptr)
	{
		_subLevel->Update(navInfo);

		if (_subLevel->SceneChanged())
		{
			_subLevel->Finalize();
			_subLevel = nullptr;
			_sceneChanged = true;
		}
		return;
	}


	//  Update player
	_player->Update(navInfo);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	int portalIndex;
	_objectInteractions->ProcessObjectInteractions(navInfo, &portalIndex);

	switch (portalIndex)
	{
	case 1:
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level01(_debug));
		_subLevel->Initialize();
		_sceneChanged = true;

		_player->Placement()->Reset();
		_player->Placement()->GoTo(6, 0, -14);
		_player->Placement()->RotateY(-90);
		break;

	case 2:
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level02(_debug));
		_subLevel->Initialize();
		_sceneChanged = true;

		_player->Placement()->Reset();
		_player->Placement()->GoTo(14, -4, -8);
		_player->Placement()->RotateY(180);
		break;

	case 3:
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level03(_debug));
		_subLevel->Initialize();
		_sceneChanged = true;

		_player->Placement()->Reset();
		_player->Placement()->GoTo(8, 0, 0);
		_player->Placement()->RotateY(90);
		break;

	case 4:
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level04(_debug));
		_subLevel->Initialize();
		_sceneChanged = true;

		_player->Placement()->Reset();
		_player->Placement()->GoTo(0, 0, -6);
		//_player->Placement()->RotateY(0);
		break;
	}

	if (portalIndex > 0)
	{


		if (portalIndex == 1)
		{
			_subLevel = std::shared_ptr<ITurboGameLevel>(new Level01(_debug));
			_subLevel->Initialize();
			_sceneChanged = true;

			_player->Placement()->GoTo(6, 0, -14);
			_player->Placement()->RotateY(-90);
		}
	}

	_scene->CameraPlacement(_player->Placement());
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
