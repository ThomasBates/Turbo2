
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
#include <TurboSceneSound.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

Level04::Level04(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboSceneObject> player,
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder,
	std::shared_ptr<MazeOptions> mazeOptions) :
	_debug(std::move(debug)),
	_sceneBuilder(std::move(sceneBuilder)),
	_mazeOptions(std::move(mazeOptions)),
	_levelState(TurboGameLevelState::Initializing),
	_player(std::move(player))
{
	_mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(CubicMazeType::Cube));
	_objectInteractions = std::shared_ptr<ICubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug));
}

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level04::Initialize()
{
	//	Create the maze.
	int mazeSize = _mazeOptions->MazeSize()->GetValue();
	_maze = _mazeFactory->MakeMaze(mazeSize, mazeSize, mazeSize);
	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithoutGravity());

	_helper = std::shared_ptr<Level00Helper>(new Level00Helper(
		_player, _maze, _motionEffects, _sceneBuilder, _objectInteractions, 
		_mazeOptions, &_keys, &_hazards));


	//	Create the exit.
	_entranceLocation = CubicMazeLocation(2, 0, 0);
	_exitLocation = CubicMazeLocation(0, 0, 2);

	_maze->Cell(_entranceLocation)->RightWall.Type = EntranceBack;


	//	Create NPC's and obstacles
	CubicMazeLocation firstKeyLocation = CubicMazeLocation(0, mazeSize - 1, 0);
	_helper->CreateKeys(&firstKeyLocation, "Key");
	_helper->CreateHazards(&_exitLocation, "Hazard");


	//	Build the scene.
//	BuildScene();

	_levelState = TurboGameLevelState::Running;
}

void Level04::Update(NavigationInfo* navInfo)
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

void Level04::BuildScene(NavigationInfo* navInfo)
{
	int keyCount = _mazeOptions->KeyCount()->GetValue();
	int required = _mazeOptions->RequiredKeyCount()->GetValue();
	uint found = keyCount - _keys.size();
	_mazeOptions->FoundKeyCount()->SetValue(found);
	bool exitLocked = found < required;

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

	_scene = _helper->BuildScene(navInfo);

	_helper->ClearSignage();
	if (keyCount == 0)
	{
		_helper->AddSignage(4, 0, 0, "You can look and go\nup and down\nas well as\nleft and right,\nforward and backward.");
	}

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->FrontWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Locked")));
	}
	else
	{
		_maze->Cell(_exitLocation)->FrontWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Exit")));
	}

	_sceneChanged = true;
}

//  Local Methods ------------------------------------------------------------------------------------------------------
