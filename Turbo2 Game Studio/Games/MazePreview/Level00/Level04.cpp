
#include <pch.h>

#include <Level04.h>

#include <ICubicMazeSceneBuilder.h>
#include <CubicMazeCornerMesh.h>
#include <CubicMazeFactory.h>
#include <CubicMazeMotionEffects_WithoutGravity.h>
#include <CubicMazeObjectInteractions.h>
#include <CubicMazeSceneObject.h>

#include <TurboGameState.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneSoundEffect.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

Level04::Level04(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboSceneObject> player,
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder,
	Level00MazeOptions mazeOptions) :
	_debug(debug),
	_player(player),
	_sceneBuilder(sceneBuilder),
	_mazeOptions(mazeOptions),
	_levelState(TurboGameLevelState::Initializing)
{
	_mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(CubicMazeType::Cube));
	_objectInteractions = std::shared_ptr<ICubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug));
}

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level04::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	gameState->SaveString("LevelInfo", "level info");
	return gameState;
}

void Level04::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	gameState->LoadString("LevelInfo", "");
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level04::Initialize()
{
	//	Create the maze.
	_maze = _mazeFactory->MakeMaze(_mazeOptions.MazeSize, _mazeOptions.MazeSize, _mazeOptions.MazeSize);
	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithoutGravity());

	_helper = std::shared_ptr<Level00Helper>(new Level00Helper(
		_player, _maze, _motionEffects, _sceneBuilder, _objectInteractions, 
		&_mazeOptions, &_keys, &_hazards));


	//	Create the exit.
	_entranceLocation = CubicMazeLocation(2, 0, 0);
	_exitLocation = CubicMazeLocation(0, 0, 2);

	_maze->Cell(_entranceLocation)->RightWall.Type = EntranceBack;


	//	Create NPC's and obstacles
	CubicMazeLocation firstKeyLocation = CubicMazeLocation(0, _mazeOptions.MazeSize - 1, 0);
	_helper->CreateKeys(&firstKeyLocation, "Key");
	_helper->CreateHazards(&_exitLocation, "Hazard");


	//	Build the scene.
	BuildScene();

	_levelState = TurboGameLevelState::Running;
}

void Level04::Update(NavigationInfo* navInfo)
{
	_sceneChanged = false;

	bool rebuildScene = _helper->Update(navInfo, &_levelState);

	if (rebuildScene)
	{
		BuildScene();
	}
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
//  Local Methods ------------------------------------------------------------------------------------------------------

void Level04::BuildScene()
{
	bool exitLocked = _keys.size() > _mazeOptions.KeyCount - _mazeOptions.RequiredKeyCount;

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->FrontWall.Type = CubicMazeCellWallType::ExitLocked;
		_maze->Cell(_exitLocation)->FrontWall.PortalIndex = 0;
	}
	else
	{
		_maze->Cell(_exitLocation)->FrontWall.Type = CubicMazeCellWallType::Exit;
		_maze->Cell(_exitLocation)->FrontWall.PortalIndex = 1;
	}

	_scene = _helper->BuildScene();

	if (_mazeOptions.LevelRound == 2)
	{
		if (exitLocked)
		{
			_helper->CreateSign(_scene, _exitLocation, CubicMazeCellWallSide::Left, "Level04Text01");
		}
		else
		{
			_helper->CreateSign(_scene, _exitLocation, CubicMazeCellWallSide::Left, "Level04Text02");
		}
	}

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->FrontWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Locked")));
	}
	else
	{
		_maze->Cell(_exitLocation)->FrontWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSoundEffect>(new TurboSceneSoundEffect("Exit")));
	}

	_sceneChanged = true;
}

//  Local Methods ------------------------------------------------------------------------------------------------------
