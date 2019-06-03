
#include <pch.h>

#include <ICubicMazeSceneBuilder.h>
#include <CubicMazeCornerMesh.h>
#include <CubicMazeMotionEffects_WithGravity.h>
#include <CubicMazeObjectInteractions.h>
#include <CubicMazeSceneObject.h>

#include <TurboGameState.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneSoundEffect.h>

#include <Level05.h>
#include <Level05CubicMazeFactory.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

Level05::Level05(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboSceneObject> player,
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder,
	Level00MazeOptions mazeOptions,
	Level00UserOptions* userOptions) :
	_debug(debug),
	_player(player),
	_sceneBuilder(sceneBuilder),
	_mazeOptions(mazeOptions),
	_userOptions(userOptions),
	_levelState(TurboGameLevelState::Initializing)
{
	_mazeFactory = std::shared_ptr<ICubicMazeFactory>(new Level05CubicMazeFactory());
	_objectInteractions = std::shared_ptr<ICubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug));
}

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level05::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	gameState->SaveString("LevelInfo", "level info");
	return gameState;
}

void Level05::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	gameState->LoadString("LevelInfo", "");
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level05::Initialize()
{
	//	Create the maze.
	_maze = _mazeFactory->MakeMaze(_mazeOptions.MazeSize, _mazeOptions.MazeSize, _mazeOptions.MazeSize);
	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithGravity(_maze));

	_helper = std::shared_ptr<Level00Helper>(new Level00Helper(
		_player, _maze, _motionEffects, _sceneBuilder, _objectInteractions,
		&_mazeOptions, &_keys, &_hazards));


	//	Create the exit.
	_soundEffectsOnLocation = CubicMazeLocation(0, 0, 0);
	_entranceLocation = CubicMazeLocation(1, 0, 0);
	_exitLocation = CubicMazeLocation(1, 0, 0);
	_invertedMouseLocation = CubicMazeLocation(2, 0, 0);


	//	Create NPC's and obstacles
	//	...


	//	Build the scene.
	BuildScene();

	_levelState = TurboGameLevelState::Running;
}

void Level05::Update(NavigationInfo* navInfo)
{
	_sceneChanged = false;


	//  Update player
	_player->Update(navInfo);
	_motionEffects->ProcessMotionEffects(navInfo, _player, true);


	//  Update NPC's and obstacles
	//	...


	//  Check for collisions
	int portalIndex = 0;
	_objectInteractions->ProcessObjectInteractions(navInfo, _maze, _player, true, &portalIndex);

	switch (portalIndex)
	{
	case 1:
		_userOptions->InvertedMouse = !_userOptions->InvertedMouse;
		_player->Placement()->Move(-2, 0, -2 + 1);
		BuildScene();
		break;

	case 2:
		_userOptions->SoundEffectsOn = !_userOptions->SoundEffectsOn;
		_player->Placement()->Move(2, 0, -2 + 1);
		BuildScene();
		break;

	case 3:
		_levelState = TurboGameLevelState::Completed;
		break;
	}
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
//  Local Methods ------------------------------------------------------------------------------------------------------

void Level05::BuildScene()
{
	_maze->Cell(_invertedMouseLocation)->BackWall.Type = CubicMazeCellWallType::Exit;
	_maze->Cell(_invertedMouseLocation)->BackWall.PortalIndex = 1;

	_maze->Cell(_soundEffectsOnLocation)->BackWall.Type = CubicMazeCellWallType::Exit;
	_maze->Cell(_soundEffectsOnLocation)->BackWall.PortalIndex = 2;

	_maze->Cell(_exitLocation)->FrontWall.Type = CubicMazeCellWallType::Exit;
	_maze->Cell(_exitLocation)->FrontWall.PortalIndex = 3;


	_scene = _helper->BuildScene();

	if (!_userOptions->InvertedMouse && !_userOptions->SoundEffectsOn)
	{
		_helper->CreateSign(_scene, _exitLocation, CubicMazeCellWallSide::Back, "Level05Text00");
	}
	else if (!_userOptions->InvertedMouse && _userOptions->SoundEffectsOn)
	{
		_helper->CreateSign(_scene, _exitLocation, CubicMazeCellWallSide::Back, "Level05Text01");
	}
	else if (_userOptions->InvertedMouse && !_userOptions->SoundEffectsOn)
	{
		_helper->CreateSign(_scene, _exitLocation, CubicMazeCellWallSide::Back, "Level05Text10");
	}
	else if (_userOptions->InvertedMouse && _userOptions->SoundEffectsOn)
	{
		_helper->CreateSign(_scene, _exitLocation, CubicMazeCellWallSide::Back, "Level05Text11");
	}


	_maze->Cell(_invertedMouseLocation)->BackWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Exit")));
	_maze->Cell(_soundEffectsOnLocation)->BackWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Exit")));
	_maze->Cell(_exitLocation)->FrontWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Exit")));

	_sceneChanged = true;
}

//  Local Methods ------------------------------------------------------------------------------------------------------
