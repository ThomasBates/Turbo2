
#include <pch.h>

#include <CubicMazeSceneBuilder.h>
#include <CubicMazeFactory.h>
#include <Level01.h>
#include <Level0Player.h>
#include <TurboGameState.h>
#include <TurboSceneMaterial.h>
#include <TurboScenePointLight.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level01::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	gameState->SaveString("LevelInfo", "level info");
	return gameState;
}

void Level01::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	gameState->LoadString("LevelInfo");
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level01::Initialize()
{
	//	Create the maze.
	std::shared_ptr<ICubicMazeFactory> mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(Layered));
	_maze = mazeFactory->MakeMaze(3, 1, 3);

	//	Create the exit.
	_maze->Cell(0, 0, 0)->BackWall.Type = EntranceBack;
	_maze->Cell(2, 0, 2)->RightWall.Type = Exit;
	_maze->Cell(2, 0, 2)->RightWall.PortalIndex = 1;


	//	Create materials.
	std::shared_ptr<ITurboSceneMaterial> cornerMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Corner"));
	std::shared_ptr<ITurboSceneMaterial> edgeMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Edge"));

	std::shared_ptr<ITurboSceneMaterial> wallMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Wall"));
	std::shared_ptr<ITurboSceneMaterial> floorMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Floor"));
	std::shared_ptr<ITurboSceneMaterial> ceilingMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Ceiling"));

	std::shared_ptr<ITurboSceneMaterial> entranceMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Entrance"));
	std::shared_ptr<ITurboSceneMaterial> entranceLockedMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00EntranceLocked"));
	std::shared_ptr<ITurboSceneMaterial> entranceBackMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00EntranceBack"));

	std::shared_ptr<ITurboSceneMaterial> exitMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Exit"));
	std::shared_ptr<ITurboSceneMaterial> exitLockedMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00ExitLocked"));
	std::shared_ptr<ITurboSceneMaterial> exitBackMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00ExitBack"));


	//	Build the scene.
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder(
		cornerMaterial, edgeMaterial,
		wallMaterial, wallMaterial, wallMaterial, wallMaterial,
		floorMaterial, ceilingMaterial,
		entranceMaterial, entranceLockedMaterial, entranceBackMaterial,
		exitMaterial, exitLockedMaterial, exitBackMaterial));
	_scene = sceneBuilder->BuildScene(_maze);

	_maze->Cell(2, 0, 2)->FrontWall.SceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level01Text01")));

	//  Create the player
	_player = std::shared_ptr<ITurboSceneObject>(new Level0Player());
	_player->Light(std::shared_ptr<ITurboSceneLight>(new TurboScenePointLight(TurboVector3D(0, 0, 0), TurboColor(1, 1, 1), 1, 1, 1)));

	//	This is easier for now.
	_scene->LightHack(false);

	//	set player Placement as camera Placement.
	_scene->CameraPlacement(_player->Placement());

	//  Create NPC's and obstacles ...
	//  ...

	//LoadLevel();

	_objectInteractions = std::shared_ptr<CubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug, _maze, _player, 0.25, 0.25, 0.25));

	_levelState = TurboGameLevelState::Running;
}

void Level01::Update(NavigationInfo navInfo)
{
	_sceneChanged = false;

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

	if (portalIndex == 1)
	{
		_levelState = TurboGameLevelState::Completed;
	}

	_scene->CameraPlacement(_player->Placement());
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
