
#include <pch.h>

#include <Level03.h>

#include <ICubicMazeSceneBuilder.h>
#include <CubicMazeCornerMesh.h>
#include <CubicMazeFactory.h>
#include <CubicMazeMotionEffects_WithoutGravity.h>
#include <CubicMazeObjectInteractions.h>
#include <CubicMazeSceneObject.h>

#include <TurboGameState.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneSound.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

Level03::Level03(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboSceneObject> player,
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder,
	Level00MazeOptions mazeOptions) :
	_debug(debug),
	_sceneBuilder(sceneBuilder),
	_mazeOptions(mazeOptions),
	_levelState(TurboGameLevelState::Initializing),
	_player(player)
{
	_mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(CubicMazeType::Cube));
	_objectInteractions = std::shared_ptr<ICubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug));
}

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> Level03::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	gameState->SaveString("LevelInfo", "level info");
	return gameState;
}

void Level03::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	gameState->LoadString("LevelInfo", "");
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level03::Initialize()
{
	//	Create the maze.
	_maze = _mazeFactory->MakeMaze(_mazeOptions.MazeSize, _mazeOptions.MazeSize, _mazeOptions.MazeSize);
	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithoutGravity());

	_helper = std::shared_ptr<Level00Helper>(new Level00Helper(
		_player, _maze, _motionEffects, _sceneBuilder, _objectInteractions,
		&_mazeOptions, &_keys, &_hazards));


	//	Create the exit.
	_entranceLocation = CubicMazeLocation(2, 2, 2);
	_exitLocation = CubicMazeLocation(0, 0, 0);

	_maze->Cell(_entranceLocation)->FrontWall.Type = EntranceBack;


	//	Create NPC's and obstacles
	CubicMazeLocation firstKeyLocation = CubicMazeLocation(0, _mazeOptions.MazeSize - 1, 0);
	_helper->CreateKeys(&firstKeyLocation, "Key");
	_helper->CreateHazards(&_exitLocation, "Hazard");


	//	Build the scene.
//	BuildScene();

	_levelState = TurboGameLevelState::Running;
}

void Level03::Update(NavigationInfo* navInfo)
{
	_sceneChanged = false;

	bool rebuildScene = _helper->Update(navInfo, &_levelState);

	if ((_scene == nullptr) || rebuildScene)
	{
		BuildScene(navInfo);
	}

	//  Update signage based on the player's location.
	_signage = _helper->GetSignage(_player);
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
//  Local Methods ------------------------------------------------------------------------------------------------------

void Level03::BuildScene(NavigationInfo* navInfo)
{
	bool exitLocked = _keys.size() > _mazeOptions.KeyCount - _mazeOptions.RequiredKeyCount;

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->LeftWall.Type = CubicMazeCellWallType::ExitLocked;
		_maze->Cell(_exitLocation)->LeftWall.PortalIndex = 0;
	}
	else
	{
		_maze->Cell(_exitLocation)->LeftWall.Type = CubicMazeCellWallType::Exit;
		_maze->Cell(_exitLocation)->LeftWall.PortalIndex = 1;
	}

	_scene = _helper->BuildScene(navInfo);

	_helper->ClearSignage();
	if (_mazeOptions.KeyCount == 0)
	{
		_helper->AddSignage(4, -4, -4, "You can look and go\nup and down\nas well as\nleft and right,\nforward and backward.");
	}

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->LeftWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Locked")));
	}
	else
	{
		_maze->Cell(_exitLocation)->LeftWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Exit")));
	}

	_sceneChanged = true;
}

//  Local Methods ------------------------------------------------------------------------------------------------------
