
#include <pch.h>

#include <CubicMazeSceneBuilder.h>
#include <CubicMazeFactory.h>
#include <Level04.h>
#include <Level0Player.h>
#include <OriginalPlayer.h>
#include <TurboGameState.h>
#include <TurboScenePointLight.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level04::State()
{
	std::shared_ptr<ITurboGameState> state = std::shared_ptr<ITurboGameState>(new TurboGameState());
	state->SaveString("LevelInfo", "level info");
	return state;
}

void Level04::State(std::shared_ptr<ITurboGameState> state)
{
	state->LoadString("LevelInfo");
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level04::Initialize()
{
	//	Create the maze.
	std::shared_ptr<ICubicMazeFactory> mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(Cube));
	_maze = mazeFactory->MakeMaze(3, 3, 3);

	//	Create the exit.
	_maze->Cell(2, 0, 0)->RightWall.Type = EntranceBack;
	_maze->Cell(0, 0, 2)->FrontWall.Type = Exit;
	_maze->Cell(0, 0, 2)->FrontWall.PortalIndex = 1;


	//	Build the scene.
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder());
	_scene = sceneBuilder->BuildScene(_maze);

	//  Create the player
	_player = std::shared_ptr<ITurboSceneObject>(new OriginalPlayer());
	_player->Light(std::shared_ptr<ITurboSceneLight>(new TurboScenePointLight(TurboVector3D(0, 0, 0), TurboColor(1, 1, 1), 1, 1, 1)));

	//	This is easier for now.
	_scene->LightHack(true);

	//	set player Placement as camera Placement.
	_scene->CameraPlacement(_player->Placement());

	//  Create NPC's and obstacles ...
	//  ...

	//LoadLevel();

	_objectInteractions = std::shared_ptr<CubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug, _maze, _player));
}

void Level04::Update(NavigationInfo navInfo)
{
	_sceneChanged = false;

	//  Update player
	_player->Update(navInfo);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	int portalIndex;
	_objectInteractions->ProcessObjectInteractions(navInfo, &portalIndex);

	if (portalIndex > 0)
	{
		if (portalIndex == 1)
		{
			_sceneChanged = true;
		}
	}

	_scene->CameraPlacement(_player->Placement());
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
