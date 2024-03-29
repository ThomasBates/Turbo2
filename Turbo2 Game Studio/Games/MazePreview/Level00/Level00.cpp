
#include <pch.h>

#include <Level00.h>
#include <Level01.h>
#include <Level02.h>
#include <Level03.h>
#include <Level04.h>
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

#include <TurboCoreHelpers.h>
#include <TurboEventHandler.h>
#include <TurboGameState.h>
#include <TurboSceneAmbientLight.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneSound.h>
#include <TurboSceneSprite.h>
#include <TurboSceneTexture.h>

using namespace Turbo::Core;
using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

Level00::Level00(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<MazePreviewGameState> gameState,
	std::shared_ptr<ITurboSceneObject> player) :
	_debug(std::move(debug)),
	_gameState(std::move(gameState)),
	_player(std::move(player))
{
	_mazeFactory = std::shared_ptr<ICubicMazeFactory>(new Level00CubicMazeFactory());
	_objectInteractions = std::shared_ptr<ICubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug));

	_mazeOptions = _gameState->Maze();

	_gameState->Game()->OnValueChanged()->Subscribe(std::shared_ptr<ITurboEventHandler<std::shared_ptr<TurboConfigValueChangedEventArgs>>>(
			new TurboEventHandler<Level00, std::shared_ptr<TurboConfigValueChangedEventArgs>>(this, &Level00::GameOptionsOnValueChanged)));
}

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> Level00::Scene()
{
	if (_subLevel != nullptr)
	{
		return _subLevel->Scene();
	}

	return _scene;
}

std::string Level00::GetSignage()
{
	if (_subLevel != nullptr)
	{
		return _subLevel->GetSignage();
	}

	return _signage;
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level00::Initialize()
{
	_signage = "Welcome\nto\nMazePreview!";

	//	Create the maze.
	_maze = _mazeFactory->MakeMaze(0, 0, 0);
	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithGravity(_maze));
	//_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithoutGravity());

	_helper = std::shared_ptr<Level00Helper>(new Level00Helper(
		_player, _maze, _motionEffects, _sceneBuilder, _objectInteractions,
		_mazeOptions, nullptr, nullptr));

	UpdateMazeOptions(_gameState->Game()->CurrentRound()->GetValue());
}

void Level00::Update(NavigationInfo* navInfo)
{
	_sceneChanged = false;

	if (_scene == nullptr)
	{
		//	Build the scene.
		BuildScene();

		//	Place the player
		_player->Placement()->Reset();
		_player->Placement()->Move(8, 0, -4);

		//  Create NPC's and obstacles ...
		//  ...
	}

	//	Check for action controls first regardless of sublevel.
	for (auto& control: navInfo->Controls)
	{
		if (control->Type() == TurboGameControlType::Action)
		{
			if (_actionStillActive && !control->IsActive())
				_actionStillActive = false;

			if (!_actionStillActive && control->IsActive() && _actionTriggered == 0)
			{
				_actionTriggered = (int)control->XValue();
				_actionStillActive = true;
			}
		}
	}

	if (_subLevel == nullptr)
	{
		if (_gameOptionsChanged)
		{
			ValidateGameOptions();
			_gameOptionsChanged = false;
			UpdateMazeOptions(_gameState->Game()->CurrentRound()->GetValue());
			BuildScene();

			_player->Placement()->Reset();

			//	Place player in view of the entrance to the current level.
			switch (_gameState->Game()->CurrentLevel()->GetValue())
			{
				case 1:
					_player->Placement()->Move(0, 0, -6);
					break;
				case 2:
					_player->Placement()->Move(6, 0, -14);
					_player->Placement()->RotateY(-90);
					break;
				case 3:
					_player->Placement()->Move(14, -4, -8);
					_player->Placement()->RotateY(-180);
					break;
				case 4:
					_player->Placement()->Move(8, 0, 0);
					_player->Placement()->RotateY(-270);
					break;
			}

			_sceneChanged = true;
		}
	}
	else
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

				_gameState->Game()->BeginBatch();
				int unlockedRound = _gameState->Game()->UnlockedRound()->GetValue();
				int unlockedLevel = _gameState->Game()->UnlockedLevel()->GetValue();
				int currentRound = _gameState->Game()->CurrentRound()->GetValue();
				int currentLevel = _gameState->Game()->CurrentLevel()->GetValue();

				switch (_subLevelIndex)
				{
					case 1:
						_player->Placement()->Move(1, dY, -10);
						if (currentRound == unlockedRound && unlockedLevel < 2)
							unlockedLevel = 2;
						currentLevel++;
						break;

					case 2:
						_player->Placement()->Move(10, dY, -9);
						if (currentRound == unlockedRound && unlockedLevel < 3)
							unlockedLevel = 3;
						currentLevel++;
						break;

					case 3:
						_player->Placement()->Move(9, dY, 0);
						if (currentRound == unlockedRound && unlockedLevel < 4)
							unlockedLevel = 4;
						currentLevel++;
						break;

					case 4:
						_player->Placement()->Move(0, dY, -1);
						if (currentRound == unlockedRound)
						{
							unlockedRound++;
							unlockedLevel = 1;
						}
						currentRound++;
						currentLevel = 1;
						UpdateMazeOptions(currentRound);
						break;
				}

				_gameState->Game()->UnlockedRound()->SetValue(unlockedRound);
				_gameState->Game()->UnlockedLevel()->SetValue(unlockedLevel);
				_gameState->Game()->CurrentRound()->SetValue(currentRound);
				_gameState->Game()->CurrentLevel()->SetValue(currentLevel);
				_gameState->Game()->EndBatch();
				_gameOptionsChanged = false;

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

	//  Update signage based on the player's location.
	_signage = _helper->GetSignage(_player);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	int portalIndex;
	_objectInteractions->ProcessObjectInteractions(navInfo, _maze, _player, true, &portalIndex);

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
			_subLevel = std::shared_ptr<ITurboGameLevel>(new Level04(_debug, _player, _sceneBuilder, _mazeOptions));
			break;

		default:
			break;
	}

	if (_subLevel != nullptr)
	{
		//_subLevel->GameState(_gameState);
		_subLevel->Initialize();
		_subLevelIndex = portalIndex;
		_sceneChanged = true;
	}
}

int Level00::Action()
{
	int result = _actionTriggered;
	_actionTriggered = 0;
	return result;
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
//  Local Methods ------------------------------------------------------------------------------------------------------

void Level00::GameOptionsOnValueChanged(void *sender, std::shared_ptr<TurboConfigValueChangedEventArgs> args)
{
	_gameOptionsChanged = true;
}

void Level00::ValidateGameOptions()
{
	_gameState->Game()->BeginBatch();
	int unlockedRound = _gameState->Game()->UnlockedRound()->GetValue();
	int unlockedLevel = _gameState->Game()->UnlockedLevel()->GetValue();
	int currentRound = _gameState->Game()->CurrentRound()->GetValue();
	int currentLevel = _gameState->Game()->CurrentLevel()->GetValue();

	if (unlockedRound < 1)
		unlockedRound = 1;

	if (currentRound < 1)
		currentRound = 1;

	if (currentRound > unlockedRound)
		currentRound = unlockedRound;

	if (unlockedLevel < 1)
		unlockedLevel = 1;

	if (unlockedLevel > 4)
		unlockedLevel = 4;

	if (currentLevel < 1)
		currentLevel = 1;

	if (currentLevel > 4)
		currentLevel = 4;

	if (currentRound == unlockedRound && currentLevel > unlockedLevel)
		currentLevel = unlockedLevel;

	_gameState->Game()->UnlockedRound()->SetValue(unlockedRound);
	_gameState->Game()->UnlockedLevel()->SetValue(unlockedLevel);
	_gameState->Game()->CurrentRound()->SetValue(currentRound);
	_gameState->Game()->CurrentLevel()->SetValue(currentLevel);
	_gameState->Game()->EndBatch();
}

void Level00::UpdateMazeOptions(int currentRound)
{
	_mazeOptions->BeginBatch();

	switch (currentRound)
	{
		case 1:
			_sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Laboratory());
			_mazeOptions->MazeSize()->SetValue(3);
			_mazeOptions->KeyCount()->SetValue(0);
			_mazeOptions->RequiredKeyCount()->SetValue(0);
			_mazeOptions->HazardCount()->SetValue(0);
			_mazeOptions->MovingKeys()->SetValue(false);
			_mazeOptions->MovingHazards()->SetValue(false);
			_mazeOptions->LightsOn()->SetValue(true);
			break;

		case 2:
			_sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Castle());
			_mazeOptions->MazeSize()->SetValue(3);
			_mazeOptions->KeyCount()->SetValue(1);
			_mazeOptions->RequiredKeyCount()->SetValue(1);
			_mazeOptions->HazardCount()->SetValue(0);
			_mazeOptions->MovingKeys()->SetValue(false);
			_mazeOptions->MovingHazards()->SetValue(false);
			_mazeOptions->LightsOn()->SetValue(true);
			break;

		case 3:
			_sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Metal());
			_mazeOptions->MazeSize()->SetValue(3);
			_mazeOptions->KeyCount()->SetValue(2);
			_mazeOptions->RequiredKeyCount()->SetValue(1);
			_mazeOptions->HazardCount()->SetValue(1);
			_mazeOptions->MovingKeys()->SetValue(true);
			_mazeOptions->MovingHazards()->SetValue(false);
			_mazeOptions->LightsOn()->SetValue(true);
			break;

		default:
			if ((currentRound % 2) == 0)
			{
				_sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Castle());
			}
			else
			{
				_sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder_Metal());
			}
			_mazeOptions->MazeSize()->SetValue(3);
			_mazeOptions->KeyCount()->SetValue(currentRound - 2);
			_mazeOptions->RequiredKeyCount()->SetValue(currentRound - 2);
			_mazeOptions->HazardCount()->SetValue(currentRound - 2);
			_mazeOptions->MovingKeys()->SetValue(true);
			_mazeOptions->MovingHazards()->SetValue(true);
			_mazeOptions->LightsOn()->SetValue(false);
			break;
	}

	_mazeOptions->EndBatch();
}

void Level00::BuildScene()
{
	_helper->ClearSignage();

	int unlockedRound = _gameState->Game()->UnlockedRound()->GetValue();
	int unlockedLevel = _gameState->Game()->UnlockedLevel()->GetValue();
	int currentRound = _gameState->Game()->CurrentRound()->GetValue();

	if (currentRound < unlockedRound || unlockedLevel >= 1)
	{
		_maze->Cell(0, 0, 4)->FrontWall.Type = CubicMazeCellWallType::Entrance;
		_maze->Cell(0, 0, 4)->FrontWall.PortalIndex = 1;

		switch (currentRound)
		{
			case 1:
				_helper->AddSignage(0, 0, -7, "A green portal\nlike this one\nis an entrance\nto a maze.\n\nGo on in.");
				break;
			case 2:
				_helper->AddSignage(0, 0, -7, "For each maze\nin this round,\nyou will need\nto find the key\nto unlock the exit.");
				break;
			case 3:
				_helper->AddSignage(0, 0, -7, "For each maze\nin this round,\nthere are two keys.\nYou only need one.\nThe hazard is\neasy to avoid.");
				break;
			case 4:
				_helper->AddSignage(0, 0, -7, "For each maze\nin this round,\nyou need both keys.\nThe hazards are\nharder to avoid.");
				break;
			default:
				_helper->AddSignage(0, 0, -7, string_format("{} keys.\n{} hazards.", currentRound - 2, currentRound - 2));
				break;
		}
	}
	else
	{
		_maze->Cell(0, 0, 4)->FrontWall.Type = CubicMazeCellWallType::EntranceLocked;
		_maze->Cell(0, 0, 4)->FrontWall.PortalIndex = 0;

		if (currentRound == 1)
			_helper->AddSignage(0, 0, -7, "This maze has not\nbeen unlocked.");
	}

	if (currentRound < unlockedRound || unlockedLevel >= 2)
	{
		_maze->Cell(4, 0, 7)->RightWall.Type = CubicMazeCellWallType::Entrance;
		_maze->Cell(4, 0, 7)->RightWall.PortalIndex = 2;

		if (currentRound == 1)
			_helper->AddSignage(7, 0, -14, "This is a harder maze.\n\nYou have to go\ndown two levels\nto find the exit.");
	}
	else
	{
		_maze->Cell(4, 0, 7)->RightWall.Type = CubicMazeCellWallType::EntranceLocked;
		_maze->Cell(4, 0, 7)->RightWall.PortalIndex = 0;

		if (currentRound == 1)
			_helper->AddSignage(7, 0, -14, "This maze has not\nbeen unlocked.");
	}

	if (currentRound < unlockedRound || unlockedLevel >= 3)
	{
		_maze->Cell(7, 2, 3)->BackWall.Type = CubicMazeCellWallType::Entrance;
		_maze->Cell(7, 2, 3)->BackWall.PortalIndex = 3;

		if (currentRound == 1)
			_helper->AddSignage(14, -4, -7, "Now we're getting\nto the fun part.\n\nThe exit is two\nlevels up, and\n   ...   \nwell, you'll see.");
	}
	else
	{
		_maze->Cell(7, 2, 3)->BackWall.Type = CubicMazeCellWallType::EntranceLocked;
		_maze->Cell(7, 2, 3)->BackWall.PortalIndex = 0;

		if (currentRound == 1)
			_helper->AddSignage(14, -4, -7, "This maze has not\nbeen unlocked.");
	}

	if (currentRound < unlockedRound || unlockedLevel >= 4)
	{
		_maze->Cell(3, 0, 0)->LeftWall.Type = CubicMazeCellWallType::Entrance;
		_maze->Cell(3, 0, 0)->LeftWall.PortalIndex = 4;

		if (currentRound == 1)
			_helper->AddSignage(7, 0, 0, "Another fun maze.");
	}
	else
	{
		_maze->Cell(3, 0, 0)->LeftWall.Type = CubicMazeCellWallType::EntranceLocked;
		_maze->Cell(3, 0, 0)->LeftWall.PortalIndex = 0;

		if (currentRound == 1)
			_helper->AddSignage(7, 0, 0, "This maze has not\nbeen unlocked.");
	}

	_scene = _sceneBuilder->BuildScene(_maze);
	_scene->AddSceneObject(_player);
	_scene->CameraPlacement(_player->Placement());

	//  Create the player
	//_player->Light(std::shared_ptr<ITurboSceneLight>(new TurboSceneAmbientLight(TurboColor(1.0, 1.0, 1.0))));
	//_player->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Exit")));

	//	This is easier for now.
	_scene->LightHack(!_mazeOptions->LightsOn()->GetValue());

	//	This looks like it could be combined with setting the portals, above,
	//	but that needs to be done before _sceneBuilder->BuildScene(_maze);
	//  and this needs to be done after *.SceneObject is defined.
	std::shared_ptr<ITurboSceneSound> entranceSound = std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Entrance"));
	std::shared_ptr<ITurboSceneSound> lockedSound = std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Locked"));

	if (currentRound < unlockedRound || unlockedLevel >= 1)
	{
		_maze->Cell(0, 0, 4)->FrontWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		_maze->Cell(0, 0, 4)->FrontWall.SceneObject->HitSound(lockedSound);
	}

	if (currentRound < unlockedRound || unlockedLevel >= 2)
	{
		_maze->Cell(4, 0, 7)->RightWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		_maze->Cell(4, 0, 7)->RightWall.SceneObject->HitSound(lockedSound);
	}

	if (currentRound < unlockedRound || unlockedLevel >= 3)
	{
		_maze->Cell(7, 2, 3)->BackWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		_maze->Cell(7, 2, 3)->BackWall.SceneObject->HitSound(lockedSound);
	}

	if (currentRound < unlockedRound || unlockedLevel >= 4)
	{
		_maze->Cell(3, 0, 0)->LeftWall.SceneObject->HitSound(entranceSound);
	}
	else
	{
		_maze->Cell(3, 0, 0)->LeftWall.SceneObject->HitSound(lockedSound);
	}
}

//  Local Methods ------------------------------------------------------------------------------------------------------
