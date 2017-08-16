
#include <pch.h>

#include <CubicMazeSceneBuilder.h>
#include <Level00.h>
#include <Level01.h>
#include <Level02.h>
#include <Level03.h>
#include <Level04.h>
#include <Level00CubicMazeFactory.h>
#include <Level01MotionEffects.h>
#include <TurboGameState.h>
#include <TurboSceneAmbientLight.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneSoundEffect.h>
#include <OriginalPlayer.h>
#include <OriginalLevel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level00::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	gameState->SaveString("LevelInfo", "level info");
	return gameState;
}

void Level00::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	gameState->LoadString("LevelInfo");
}

std::shared_ptr<ITurboScene> Level00::Scene()
{
	if (_subLevel != nullptr)
	{
		return _subLevel->Scene();
	}

	return _scene;
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level00::Initialize()
{
	//	Create the maze.
	std::shared_ptr<ICubicMazeFactory> mazeFactory = std::shared_ptr<ICubicMazeFactory>(new Level00CubicMazeFactory());
	_maze = mazeFactory->MakeMaze(0, 0, 0);

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
	_sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder(
		cornerMaterial, edgeMaterial,
		wallMaterial, wallMaterial, wallMaterial, wallMaterial, 
		floorMaterial, ceilingMaterial,
		entranceMaterial, entranceLockedMaterial, entranceBackMaterial,
		exitMaterial, exitLockedMaterial, exitBackMaterial));

	_scene = BuildScene(_maze);

	//  Create the player
	//_player->Light(std::shared_ptr<ITurboSceneLight>(new TurboSceneAmbientLight(TurboColor(1.0, 1.0, 1.0))));
	//_player->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Exit")));

	//	This is easier for now.
	_scene->LightHack(false);

	//	Place the player
	_player->Placement()->Reset();
	_player->Placement()->Move(8, 0, -4);

	//  Create NPC's and obstacles ...
	//  ...

	_scene->CameraPlacement(_player->Placement());

	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new Level01MotionEffects());
	_objectInteractions = std::shared_ptr<CubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug, _maze, 0.25, 0.25, 0.25));
}

void Level00::Update(NavigationInfo navInfo)
{
	_sceneChanged = false;

	if (_subLevel != nullptr)
	{
		_subLevel->Update(navInfo);

		TurboGameLevelState levelState = _subLevel->LevelState();

		switch (levelState)
		{
		case TurboGameLevelState::Failed:
			_player->Placement()->Reset();

			//	Send player back to just outside the entrance to the failed level.
			switch (_subLevelIndex)
			{
			case 1:
				_player->Placement()->Move(0, 0, -8);
				break;
			case 2:
				_player->Placement()->Move(8, 0, -14);
				_player->Placement()->RotateY(-90);
				break;
			case 3:
				_player->Placement()->Move(14, -4, -6);
				_player->Placement()->RotateY(-180);
				break;
			case 4:
				_player->Placement()->Move(6, 0, 0);
				_player->Placement()->RotateY(-270);
				break;
			}

			_subLevel->Finalize();
			_subLevel = nullptr;
			_subLevelIndex = 0;
			_scene = BuildScene(_maze);
			_sceneChanged = true;
			break;

		case TurboGameLevelState::Completed:
			_player->Placement(_subLevel->Player()->Placement());

			//	Center player vertically when they return to main level.
			double fromY = _player->Placement()->Position().Y;
			double toY = round(fromY * 0.5) * 2.0;
			double dY = toY - fromY;

			switch (_subLevelIndex)
			{
			case 1:
				_player->Placement()->Move(1, dY, -10);
				_level02Unlocked = true;
				break;
			case 2:
				_player->Placement()->Move(10, dY, -9);
				_level03Unlocked = true;
				break;
			case 3:
				_player->Placement()->Move(9, dY, 0);
				_level04Unlocked = true;
				break;
			case 4:
				_player->Placement()->Move(0, dY, -1);
				_level04Unlocked = true;
				break;
			}

			_subLevel->Finalize();
			_subLevel = nullptr;
			_subLevelIndex = 0;
			_scene = BuildScene(_maze);
			_sceneChanged = true;
			break;
		}

		if ((_subLevel != nullptr) && (_subLevel->SceneChanged()))
		{
			_sceneChanged = true;
		}

		return;
	}


	//  Update player
	_player->Update(navInfo);
	_motionEffects->ProcessMotionEffects(navInfo, _player, true);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	int portalIndex;
	_objectInteractions->ProcessObjectInteractions(navInfo, _player, &portalIndex, true);

	//	Share the placement object so there is continuity of motion when going through the portals.
	//	Move the player by the amount of offset between levels, plus 1m to jump past the gap in the portal.
	switch (portalIndex)
	{
	case 1:
		//_maze->Cell(0, 0, 4)->FrontWall.SceneObject->PlaySound(_player->Placement());
		_player->Placement()->Move(0, 0, 10 - 1);
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level01(_debug, _player));
		_subLevel->Initialize();
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
		break;

	case 2:
		_player->Placement()->Move(-10 + 1, 0, 10);
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level02(_debug, _player));
		_subLevel->Initialize();
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
		break;

	case 3:
		_player->Placement()->Move(-10, 0, 1);
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level03(_debug, _player));
		_subLevel->Initialize();
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
		break;

	case 4:
		_player->Placement()->Move(-1, 0, 0);
		_subLevel = std::shared_ptr<ITurboGameLevel>(new Level04(_debug, _player));
		_subLevel->Initialize();
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
		break;
	}
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
//  Local Methods ------------------------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> Level00::BuildScene(std::shared_ptr<CubicMaze> cubicMaze)
{
	if (_level01Unlocked)
	{
		cubicMaze->Cell(0, 0, 4)->FrontWall.Type = CubicMazeCellWallType::Entrance;
		cubicMaze->Cell(0, 0, 4)->FrontWall.PortalIndex = 1;
	}

	if (_level02Unlocked)
	{
		cubicMaze->Cell(4, 0, 7)->RightWall.Type = CubicMazeCellWallType::Entrance;
		cubicMaze->Cell(4, 0, 7)->RightWall.PortalIndex = 2;
	}

	if (_level03Unlocked)
	{
		cubicMaze->Cell(7, 2, 3)->BackWall.Type = CubicMazeCellWallType::Entrance;
		cubicMaze->Cell(7, 2, 3)->BackWall.PortalIndex = 3;
	}

	if (_level04Unlocked)
	{
		cubicMaze->Cell(3, 0, 0)->LeftWall.Type = CubicMazeCellWallType::Entrance;
		cubicMaze->Cell(3, 0, 0)->LeftWall.PortalIndex = 4;
	}

	std::shared_ptr<ITurboScene> scene = _sceneBuilder->BuildScene(cubicMaze);
	scene->AddSceneObject(_player);
	scene->CameraPlacement(_player->Placement());

	std::shared_ptr<ITurboSceneSoundEffect> entranceSound = std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Entrance"));
	std::shared_ptr<ITurboSceneSoundEffect> lockedSound = std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Locked"));

	cubicMaze->Cell(4, 0, 3)->FrontWall.SceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Text00")));

	if (_level01Unlocked)
	{
		cubicMaze->Cell(0, 0, 4)->LeftWall.SceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Text01")));
		cubicMaze->Cell(0, 0, 4)->FrontWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		cubicMaze->Cell(0, 0, 4)->FrontWall.SceneObject->HitSound(lockedSound);
	}

	if (_level02Unlocked)
	{
		cubicMaze->Cell(4, 0, 7)->FrontWall.SceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Text02")));
		cubicMaze->Cell(4, 0, 7)->RightWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		cubicMaze->Cell(4, 0, 7)->RightWall.SceneObject->HitSound(lockedSound);
	}

	if (_level03Unlocked)
	{
		cubicMaze->Cell(7, 2, 3)->RightWall.SceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Text03")));
		cubicMaze->Cell(7, 2, 3)->BackWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		cubicMaze->Cell(7, 2, 3)->BackWall.SceneObject->HitSound(lockedSound);
	}

	if (_level04Unlocked)
	{
		cubicMaze->Cell(3, 0, 0)->BackWall.SceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("Level00Text04")));
		cubicMaze->Cell(3, 0, 0)->LeftWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		cubicMaze->Cell(3, 0, 0)->LeftWall.SceneObject->HitSound(lockedSound);
	}

	return scene;
}

//  Local Methods ------------------------------------------------------------------------------------------------------
