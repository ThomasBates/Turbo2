
#include <pch.h>

#include <Level00.h>
#include <Level01.h>
#include <Level02.h>
#include <Level03.h>
#include <Level04.h>
#include <Level05.h>
#include <Level00CubicMazeFactory.h>
#include <CubicMazeMotionEffects_WithGravity.h>
#include <CubicMazeMotionEffects_WithoutGravity.h>
#include <CubicMazeSceneBuilder_Castle.h>
#include <CubicMazeSceneBuilder_Flat.h>
#include <CubicMazeSceneBuilder_Laboratory.h>
#include <CubicMazeSceneBuilder_Metal.h>
#include <CubicMazeSceneBuilder_Random.h>
#include <CubicMazeSceneObject.h>
#include <CubicMazeSignMesh.h>
#include <TurboGameState.h>
#include <TurboSceneAmbientLight.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneSoundEffect.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

Level00::Level00(
	std::shared_ptr<ITurboDebug> debug, 
	std::shared_ptr<ITurboSceneObject> player,
	Level00UserOptions* userOptions) :
	_debug(debug),
	_player(player),
	_userOptions(userOptions)
{
	_mazeFactory = std::shared_ptr<ICubicMazeFactory>(new Level00CubicMazeFactory());
	_objectInteractions = std::shared_ptr<ICubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug));
}

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level00::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = nullptr;

	if (_subLevel != nullptr)
	{
		gameState = _subLevel->GameState();
	}

	if (gameState == nullptr)
	{
		gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	}

	gameState->SaveInteger("Level00.LevelRound", _mazeOptions.LevelRound);

	gameState->SaveBoolean("Level00.Level02Unlocked", _level02Unlocked);
	gameState->SaveBoolean("Level00.Level03Unlocked", _level03Unlocked);
	gameState->SaveBoolean("Level00.Level04Unlocked", _level04Unlocked);

	return gameState;
}

void Level00::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	_gameState = gameState;

	if (gameState == nullptr)
	{
		return;
	}

	_mazeOptions.LevelRound = gameState->LoadInteger("Level00.LevelRound", 1);
	if (_mazeOptions.LevelRound < 1)
	{
		_mazeOptions.LevelRound = 1;
	}
	
	_level01Unlocked = true;
	_level02Unlocked = gameState->LoadBoolean("Level00.Level02Unlocked", false);
	_level03Unlocked = gameState->LoadBoolean("Level00.Level03Unlocked", false);
	_level04Unlocked = gameState->LoadBoolean("Level00.Level04Unlocked", false);

	if (_subLevel != nullptr)
	{
		_subLevel->GameState(gameState);
	}
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
	_maze = _mazeFactory->MakeMaze(0, 0, 0);
	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithGravity(_maze));
	//_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithoutGravity());

	_helper = std::shared_ptr<Level00Helper>(new Level00Helper(
		_player, _maze, _motionEffects, _sceneBuilder, _objectInteractions,
		&_mazeOptions, NULL, NULL));

	//_mazeOptions.LevelRound = 1;
	_previewMazeOptions.LevelRound = _mazeOptions.LevelRound + 1;
	UpdateMazeOptions(&_sceneBuilder, &_mazeOptions);
	UpdateMazeOptions(&_previewSceneBuilder, &_previewMazeOptions);

	//	Build the scene.
	BuildScene();

	//	Place the player
	_player->Placement()->Reset();
	_player->Placement()->Move(8, 0, -4);

	//  Create NPC's and obstacles ...
	//  ...
}

void Level00::Update(NavigationInfo* navInfo)
{
	_sceneChanged = false;

	if (_subLevel != nullptr)
	{
		_subLevel->Update(navInfo);

		TurboGameLevelState levelState = _subLevel->LevelState();

		switch (levelState)
		{
			default:
				break;

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
				BuildScene();
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
						_level02Unlocked = false;
						_level03Unlocked = false;
						_level04Unlocked = false;
						_mazeOptions.LevelRound++;
						_previewMazeOptions.LevelRound++;
						UpdateMazeOptions(&_sceneBuilder, &_mazeOptions);
						UpdateMazeOptions(&_previewSceneBuilder, &_previewMazeOptions);
						break;

					case 5:
						_player->Placement()->Move(6, dY, -8);
						break;
				}

				_subLevel->Finalize();
				_subLevel = nullptr;
				_subLevelIndex = 0;
				BuildScene();
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
	_objectInteractions->ProcessObjectInteractions(navInfo, _maze, _player, true, &portalIndex);

//	_player->Placement()->Reset();
//	_player->Placement()->GoTo(8, 0, -4);

	//	Share the player object so there is continuity of motion when going through the portals.
	//	Move the player by the amount of offset between levels, plus 1m to jump past the gap in the portal.
	switch (portalIndex)
	{
		case 1:
			_player->Placement()->Move(0, 0, 10 - 1);
			_subLevel = std::shared_ptr<ITurboGameLevel>(new Level01(_debug, _player, _sceneBuilder, _mazeOptions));
			break;

		case 2:
			_player->Placement()->Move(-10 + 1, 0, 10);
			_subLevel = std::shared_ptr<ITurboGameLevel>(new Level02(_debug, _player, _sceneBuilder, _mazeOptions));
			break;

		case 3:
			_player->Placement()->Move(-10, 0, 1);
			_subLevel = std::shared_ptr<ITurboGameLevel>(new Level03(_debug, _player, _sceneBuilder, _mazeOptions));
			break;

		case 4:
			_player->Placement()->Move(-1, 0, 0);
			_subLevel = std::shared_ptr<ITurboGameLevel>(new Level04(_debug, _player, _previewSceneBuilder, _previewMazeOptions));
			break;

		case 5:
			_player->Placement()->Move(-6, 0, 8 - 1);
			_subLevel = std::shared_ptr<ITurboGameLevel>(new Level05(_debug, _player, _sceneBuilder, _mazeOptions, _userOptions));
			break;
	}

	if (_subLevel != nullptr)
	{
		_subLevel->GameState(_gameState);
		_subLevel->Initialize();
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
	}
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
//  Local Methods ------------------------------------------------------------------------------------------------------

void Level00::UpdateMazeOptions(std::shared_ptr<ICubicMazeSceneBuilder>* sceneBuilder, Level00MazeOptions* mazeOptions)
{
	switch (mazeOptions->LevelRound)
	{
		case 1:
			*sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Laboratory());
			mazeOptions->MazeSize = 3;
			mazeOptions->KeyCount = 0;
			mazeOptions->RequiredKeyCount = 0;
			mazeOptions->HazardCount = 0;
			mazeOptions->MovingKeys = false;
			mazeOptions->MovingHazards = false;
			mazeOptions->LightsOn = true;
			break;

		case 2:
			*sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Castle());
			mazeOptions->MazeSize = 3;
			mazeOptions->KeyCount = 1;
			mazeOptions->RequiredKeyCount = 1;
			mazeOptions->HazardCount = 0;
			mazeOptions->MovingKeys = false;
			mazeOptions->MovingHazards = false;
			mazeOptions->LightsOn = true;
			break;

		case 3:
			*sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Metal());
			mazeOptions->MazeSize = 3;
			mazeOptions->KeyCount = 2;
			mazeOptions->RequiredKeyCount = 1;
			mazeOptions->HazardCount = 1;
			mazeOptions->MovingKeys = true;
			mazeOptions->MovingHazards = false;
			mazeOptions->LightsOn = true;
			break;

		default:
			if ((mazeOptions->LevelRound % 2) == 0)
			{
				*sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Castle());
			}
			else
			{
				*sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Metal());
			}
			mazeOptions->MazeSize = 3;
			mazeOptions->KeyCount = mazeOptions->LevelRound - 2;
			mazeOptions->RequiredKeyCount = mazeOptions->LevelRound - 2;
			mazeOptions->HazardCount = mazeOptions->LevelRound - 2;
			mazeOptions->MovingKeys = true;
			mazeOptions->MovingHazards = true;
			mazeOptions->LightsOn = false;
			break;
	}
	*sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Flat());
}

void Level00::BuildScene()
{
	if (_level01Unlocked)
	{
		_maze->Cell(0, 0, 4)->FrontWall.Type = CubicMazeCellWallType::Entrance;
		_maze->Cell(0, 0, 4)->FrontWall.PortalIndex = 1;
	}
	else
	{
		_maze->Cell(0, 0, 4)->FrontWall.Type = CubicMazeCellWallType::EntranceLocked;
		_maze->Cell(0, 0, 4)->FrontWall.PortalIndex = 0;
	}

	if (_level02Unlocked)
	{
		_maze->Cell(4, 0, 7)->RightWall.Type = CubicMazeCellWallType::Entrance;
		_maze->Cell(4, 0, 7)->RightWall.PortalIndex = 2;
	}
	else
	{
		_maze->Cell(4, 0, 7)->RightWall.Type = CubicMazeCellWallType::EntranceLocked;
		_maze->Cell(4, 0, 7)->RightWall.PortalIndex = 0;
	}

	if (_level03Unlocked)
	{
		_maze->Cell(7, 2, 3)->BackWall.Type = CubicMazeCellWallType::Entrance;
		_maze->Cell(7, 2, 3)->BackWall.PortalIndex = 3;
	}
	else
	{
		_maze->Cell(7, 2, 3)->BackWall.Type = CubicMazeCellWallType::EntranceLocked;
		_maze->Cell(7, 2, 3)->BackWall.PortalIndex = 0;
	}

	if (_level04Unlocked)
	{
		_maze->Cell(3, 0, 0)->LeftWall.Type = CubicMazeCellWallType::Entrance;
		_maze->Cell(3, 0, 0)->LeftWall.PortalIndex = 4;
	}
	else
	{
		_maze->Cell(3, 0, 0)->LeftWall.Type = CubicMazeCellWallType::EntranceLocked;
		_maze->Cell(3, 0, 0)->LeftWall.PortalIndex = 0;
	}

	_maze->Cell(4, 0, 4)->BackWall.Type = CubicMazeCellWallType::Entrance;
	_maze->Cell(4, 0, 4)->BackWall.PortalIndex = 5;

	_scene = _sceneBuilder->BuildScene(_maze);
	_scene->AddSceneObject(_player);
	_scene->CameraPlacement(_player->Placement());

	//  Create the player
	//_player->Light(std::shared_ptr<ITurboSceneLight>(new TurboSceneAmbientLight(TurboColor(1.0, 1.0, 1.0))));
	//_player->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Exit")));

	//	This is easier for now.
	_scene->LightHack(!_mazeOptions.LightsOn);


	if (_mazeOptions.LevelRound == 1)
	{
		_helper->CreateSign(_scene, CubicMazeLocation(4, 0, 3), CubicMazeCellWallSide::Front, "Level00Text00");
		_helper->CreateSign(_scene, CubicMazeLocation(0, -1, 4), CubicMazeCellWallSide::Front, "Level00Text01");

		if (_level01Unlocked)
		{
			_helper->CreateSign(_scene, CubicMazeLocation(0, 0, 4), CubicMazeCellWallSide::Left, "Level00Text00");
		}

		if (_level02Unlocked)
		{
			_helper->CreateSign(_scene, CubicMazeLocation(4, 0, 7), CubicMazeCellWallSide::Front, "Level00Text02");
		}

		if (_level03Unlocked)
		{
			_helper->CreateSign(_scene, CubicMazeLocation(7, 2, 3), CubicMazeCellWallSide::Right, "Level00Text03");
		}
	}

	if (_level04Unlocked)
	{
		switch (_mazeOptions.LevelRound)
		{
		case 1:
			_helper->CreateSign(_scene, CubicMazeLocation(3, 0, 0), CubicMazeCellWallSide::Back, "Level00Text04R1");
			break;
		case 2:
			_helper->CreateSign(_scene, CubicMazeLocation(3, 0, 0), CubicMazeCellWallSide::Back, "Level00Text04R2");
			break;
		case 3:
			_helper->CreateSign(_scene, CubicMazeLocation(3, 0, 0), CubicMazeCellWallSide::Back, "Level00Text04R3");
			break;
		default:
			_helper->CreateSign(_scene, CubicMazeLocation(3, 0, 0), CubicMazeCellWallSide::Back, "Level00Text04R4");
			break;
		}
	}

	std::shared_ptr<ITurboSceneSoundEffect> entranceSound = std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Entrance"));
	std::shared_ptr<ITurboSceneSoundEffect> lockedSound = std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Locked"));

	return;

	if (_level01Unlocked)
	{
		//_maze->Cell(0, 0, 4)->FrontWall.SceneObject->HitSound(entranceSound);
		CubicMazeCell *cell = _maze->Cell(0,0,4);
		CubicMazeCellWall *wall = &(cell->FrontWall);
		ITurboSceneObject *sceneObject = wall->SceneObject;
		sceneObject->HitSound(entranceSound);
	}
	else
	{
		_maze->Cell(0, 0, 4)->FrontWall.SceneObject->HitSound(lockedSound);
	}

	if (_level02Unlocked)
	{
		_maze->Cell(4, 0, 7)->RightWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		_maze->Cell(4, 0, 7)->RightWall.SceneObject->HitSound(lockedSound);
	}

	if (_level03Unlocked)
	{
		_maze->Cell(7, 2, 3)->BackWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		_maze->Cell(7, 2, 3)->BackWall.SceneObject->HitSound(lockedSound);
	}

	if (_level04Unlocked)
	{
		_maze->Cell(3, 0, 0)->LeftWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		_maze->Cell(3, 0, 0)->LeftWall.SceneObject->HitSound(lockedSound);
	}

	_maze->Cell(4, 0, 4)->BackWall.SceneObject->HitSound(entranceSound);
}

//  Local Methods ------------------------------------------------------------------------------------------------------
