
#include <pch.h>

#include <CubicMazeSceneBuilder.h>
#include <CubicMazeFactory.h>
#include <Level03.h>
#include <Level03MotionEffects.h>
#include <OriginalPlayer.h>
#include <TurboGameState.h>
#include <TurboSceneMaterial.h>
#include <TurboScenePointLight.h>
#include <TurboSceneSoundEffect.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level03::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	gameState->SaveString("LevelInfo", "level info");
	return gameState;
}

void Level03::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	gameState->LoadString("LevelInfo");
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level03::Initialize()
{
	//	Create the maze.
	std::shared_ptr<ICubicMazeFactory> mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(Cube));
	_maze = mazeFactory->MakeMaze(3, 3, 3);

	//	Create the exit.
	_maze->Cell(2, 2, 2)->FrontWall.Type = EntranceBack;
	_maze->Cell(0, 0, 0)->LeftWall.Type = Exit;
	_maze->Cell(0, 0, 0)->LeftWall.PortalIndex = 1;


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
	_scene->AddSceneObject(_player);

	_maze->Cell(0, 0, 0)->LeftWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Exit")));

	//  Create the player
	//_player->Light(std::shared_ptr<ITurboSceneLight>(new TurboScenePointLight(TurboVector3D(0, 0, 0), TurboColor(1, 1, 1), 1, 1, 1)));
	//_player->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Entrance")));

	//	This is easier for now.
	_scene->LightHack(false);

	//	set player Placement as camera Placement.
	_scene->CameraPlacement(_player->Placement());

	//  Create NPC's and obstacles ...
	//  ...

	//LoadLevel();

	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new Level03MotionEffects());
	_objectInteractions = std::shared_ptr<CubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug, _maze, 0.25, 0.25, 0.25));

	//_player->PlaySound(1);
}

void Level03::Update(NavigationInfo navInfo)
{
	_sceneChanged = false;

	//  Update player
	_player->Update(navInfo);
	_motionEffects->ProcessMotionEffects(navInfo, _player, true);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	int portalIndex;
	_objectInteractions->ProcessObjectInteractions(navInfo, _player, &portalIndex, true);

	if (portalIndex > 0)
	{
		if (portalIndex == 1)
		{
			_levelState = TurboGameLevelState::Completed;
		}
	}

	_scene->CameraPlacement(_player->Placement());
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
