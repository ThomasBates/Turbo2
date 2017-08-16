
#include <pch.h>

#include <CubicMazeCornerMesh.h>
#include <CubicMazeFactory.h>
#include <CubicMazeSceneBuilder.h>
#include <CubicMazeSceneObject.h>

#include <Level04.h>
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

std::shared_ptr<ITurboGameState> Level04::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	gameState->SaveString("LevelInfo", "level info");
	return gameState;
}

void Level04::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	gameState->LoadString("LevelInfo");
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level04::Initialize()
{
	//	Create the maze.
	std::shared_ptr<ICubicMazeFactory> mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(CubicMazeType::Cube));
	_maze = mazeFactory->MakeMaze(3, 3, 3);

	//	Create the exit.
	_maze->Cell(2, 0, 0)->RightWall.Type = EntranceBack;
	_maze->Cell(0, 0, 2)->FrontWall.Type = ExitLocked;


	//	Create materials.
	std::shared_ptr<ITurboSceneMaterial> cornerMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Corner"));
	std::shared_ptr<ITurboSceneMaterial> edgeMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Edge"));

	std::shared_ptr<ITurboSceneMaterial> wallMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Wall"));
	std::shared_ptr<ITurboSceneMaterial> floorMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Floor"));
	std::shared_ptr<ITurboSceneMaterial> ceilingMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Ceiling"));

	std::shared_ptr<ITurboSceneMaterial> entranceMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Entrance"));
	std::shared_ptr<ITurboSceneMaterial> entranceLockedMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04EntranceLocked"));
	std::shared_ptr<ITurboSceneMaterial> entranceBackMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04EntranceBack"));

	std::shared_ptr<ITurboSceneMaterial> exitMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Exit"));
	std::shared_ptr<ITurboSceneMaterial> exitLockedMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04ExitLocked"));
	std::shared_ptr<ITurboSceneMaterial> exitBackMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04ExitBack"));


	//	Build the scene.
	_sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder(
		cornerMaterial, edgeMaterial,
		wallMaterial, wallMaterial, wallMaterial, wallMaterial,
		floorMaterial, ceilingMaterial,
		entranceMaterial, entranceLockedMaterial, entranceBackMaterial,
		exitMaterial, exitLockedMaterial, exitBackMaterial));

	_scene = BuildScene(_maze);

	//  Create the player
	//_player->Light(std::shared_ptr<ITurboSceneLight>(new TurboScenePointLight(TurboVector3D(0, 0, 0), TurboColor(1, 1, 1), 1, 1, 1)));
	//_player->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Entrance")));

	//	This is easier for now.
	_scene->LightHack(false);

	//	set player Placement as camera Placement.
	_scene->CameraPlacement(_player->Placement());

	////  Create NPC's and obstacles ...
	//_key = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(
	//	std::shared_ptr<ITurboSceneMesh>(new CubicMazeCornerMesh()),
	//	std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Key"))));
	//_key->Placement()->Move(2, -4, -2);
	//_key->Placement()->AngularVelocity(TurboVector3D(30, 30, 30));
	//_scene->AddSceneObject(_key);

	//_hazard = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(
	//	std::shared_ptr<ITurboSceneMesh>(new CubicMazeCornerMesh()),
	//	std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Hazard"))));
	//_hazard->Placement()->Move(0, 0, -4);
	//_hazard->Placement()->AngularVelocity(TurboVector3D(30, 30, 30));
	//_scene->AddSceneObject(_hazard);

	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new Level03MotionEffects());
	_objectInteractions = std::shared_ptr<CubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug, _maze, 0.25, 0.25, 0.25));

	_levelState = TurboGameLevelState::Running;

	//_player->PlaySound(1);
}

void Level04::Update(NavigationInfo navInfo)
{
	int portalIndex = 0;
	_sceneChanged = false;

	//  Update player
	_player->Update(navInfo);
	_motionEffects->ProcessMotionEffects(navInfo, _player, true);

	//  Update NPC's and obstacles
	if (_key != nullptr)
	{
		//_key->Update(navInfo);
		_key->Placement()->Rotate(_key->Placement()->AngularVelocity() * navInfo.DeltaTime);

		TurboVector3D keyPosition = _key->Placement()->Position();
		TurboVector3D playerPosition = _player->Placement()->Position();
		if ((keyPosition - playerPosition).Length() < 0.25)
		{
			_drawKey = false;
			_key = nullptr;
			_scene = BuildScene(_maze);
			_sceneChanged = true;

			_player->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Key")));
			_player->PlaySound(1);
		}
	}

	if (_hazard != nullptr)
	{
		NavigationInfo hazardNavInfo;
		hazardNavInfo.Time = navInfo.Time;
		hazardNavInfo.DeltaTime = navInfo.DeltaTime;

		//_hazard->Update(hazardNavInfo);
		_hazard->Placement()->Rotate(_hazard->Placement()->AngularVelocity() * hazardNavInfo.DeltaTime);
		_hazard->Placement()->Velocity(_hazard->Placement()->Velocity().Normalize());
		_objectInteractions->ProcessObjectInteractions(hazardNavInfo, _hazard, &portalIndex, false);

		TurboVector3D hazardPosition = _hazard->Placement()->Position();
		TurboVector3D playerPosition = _player->Placement()->Position();
		if ((hazardPosition - playerPosition).Length() < 0.25)
		{
			_levelState = TurboGameLevelState::Failed;

			_player->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Fail")));
			_player->PlaySound(1);
		}
	}

	//  Check for collisions
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
//  Local Methods ------------------------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> Level04::BuildScene(std::shared_ptr<CubicMaze> cubicMaze)
{

	//  Create NPC's and obstacles ...
	if (_drawKey)
	{
		_maze->Cell(0, 0, 2)->FrontWall.Type = ExitLocked;
		_maze->Cell(0, 0, 2)->FrontWall.PortalIndex = 0;
	}
	else
	{
		_maze->Cell(0, 0, 2)->FrontWall.Type = Exit;
		_maze->Cell(0, 0, 2)->FrontWall.PortalIndex = 1;
	}

	std::shared_ptr<ITurboScene> scene = _sceneBuilder->BuildScene(cubicMaze);
	scene->AddSceneObject(_player);

	//	This is easier for now.
	scene->LightHack(false);

	if (_drawKey)
	{
		_key = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(
			std::shared_ptr<ITurboSceneMesh>(new CubicMazeCornerMesh()),
			std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Key"))));
		_key->Placement()->Move(2, -4, -2);
		//_key->Placement()->Move(2, 0, 0);
		_key->Placement()->AngularVelocity(TurboVector3D(30, 30, 30));
		scene->AddSceneObject(_key);

		cubicMaze->Cell(0, 0, 2)->LeftWall.SceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Text01")));
		cubicMaze->Cell(0, 0, 2)->FrontWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Locked")));
	}
	else
	{
		cubicMaze->Cell(0, 0, 2)->LeftWall.SceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Text02")));
		cubicMaze->Cell(0, 0, 2)->FrontWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Exit")));
	}

	if (_drawHazard)
	{
		_hazard = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(
			std::shared_ptr<ITurboSceneMesh>(new CubicMazeCornerMesh()),
			std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level04Hazard"))));
		_hazard->Placement()->Move(0, 0, -4);
		_hazard->Placement()->Velocity(TurboVector3D(1, 1, 1).Normalize());
		_hazard->Placement()->AngularVelocity(TurboVector3D(30, 30, 30));
		scene->AddSceneObject(_hazard);
	}

	return scene;
}

//  Local Methods ------------------------------------------------------------------------------------------------------
