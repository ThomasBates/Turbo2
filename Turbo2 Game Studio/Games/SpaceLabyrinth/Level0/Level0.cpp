
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
#include <TurboSceneAmbientLight.h>
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
	_player->Light(std::shared_ptr<ITurboSceneLight>(new TurboSceneAmbientLight(TurboColor(1.0, 1.0, 1.0))));

	//	This is easier for now.
	_scene->LightHack(false);

	//	Place the player
	_player->Placement()->GoTo(8, 0, -6);

	//  Create NPC's and obstacles ...
	//  ...

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
			_player = std::shared_ptr<ITurboSceneObject>(new Level0Player());
			_player->Placement(_subLevel->Player()->Placement());

			switch (_subLevelIndex)
			{
			case 1:
				_player->Placement()->Move(1, 0, -10);
				break;
			case 2:
				_player->Placement()->Move(10, 0, -9);
				break;
			case 3:
				_player->Placement()->Move(9, 0, 0);
				break;
			case 4:
				_player->Placement()->Move(0, 0, -1);
				break;
			}

			_subLevel->Finalize();
			_subLevel = nullptr;
			_subLevelIndex = 0;
			_sceneChanged = true;
		}
		return;
	}


	//  Update player
	_player->Update(navInfo);

	//  Limit motion to horizontal plane for this level
	TurboVector3D velocity = _player->Placement()->Velocity();
	velocity.Y = 0;
	_player->Placement()->Velocity(velocity);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	int portalIndex;
	_objectInteractions->ProcessObjectInteractions(navInfo, &portalIndex);

	//	Share the placement object so there is continuity of motion when going through the portals.
	//	Move the player by the amount of offset between levels, plus 1m to jump past the gap in the portal.
	switch (portalIndex)
	{
	case 1:
		_player->Placement()->Move(0, 0, 10 - 1);
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level01(_debug));
		_subLevel->Initialize();
		_subLevel->Player()->Placement(_player->Placement());
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
		break;

	case 2:
		_player->Placement()->Move(-10 + 1, 0, 10);
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level02(_debug));
		_subLevel->Initialize();
		_subLevel->Player()->Placement(_player->Placement());
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
		break;

	case 3:
		_player->Placement()->Move(-10, 0, 1);
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level03(_debug));
		_subLevel->Initialize();
		_subLevel->Player()->Placement(_player->Placement());
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
		break;

	case 4:
		_player->Placement()->Move(-1, 0, 0);
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level04(_debug));
		_subLevel->Initialize();
		_subLevel->Player()->Placement(_player->Placement());
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
		break;
	}

	_scene->CameraPlacement(_player->Placement());
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
