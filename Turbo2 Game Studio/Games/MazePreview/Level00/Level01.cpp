
#include <pch.h>

#include <Level01.h>

#include <ICubicMazeSceneBuilder.h>
#include <CubicMazeCornerMesh.h>
#include <CubicMazeFactory.h>
#include <CubicMazeMotionEffects_WithGravity.h>
#include <CubicMazeObjectInteractions.h>
#include <CubicMazeSceneObject.h>

#include <TurboGameState.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneSound.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

Level01::Level01(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboSceneObject> player,
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder,
	std::shared_ptr<MazeOptions>  mazeOptions) :
		_debug(std::move(debug)),
		_sceneBuilder(std::move(sceneBuilder)),
		_mazeOptions(std::move(mazeOptions)),
		_levelState(TurboGameLevelState::Initializing),
		_player(std::move(player))
{
	_mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(CubicMazeType::Layered));
	_objectInteractions = std::shared_ptr<ICubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug));
}

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level01::Initialize()
{
	//	Create the maze.
	int mazeSize = _mazeOptions->MazeSize()->GetValue();
	_maze = _mazeFactory->MakeMaze(mazeSize, 1, mazeSize);
	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithGravity(_maze));

	_helper = std::shared_ptr<Level00Helper>(new Level00Helper(
		_player, _maze, _motionEffects, _sceneBuilder, _objectInteractions,
		_mazeOptions, &_keys, &_hazards));


	//	Create the exit.
	_entranceLocation = CubicMazeLocation(0, 0, 0);
	_exitLocation = CubicMazeLocation(2, 0, 2);

	_maze->Cell(_entranceLocation)->BackWall.Type = EntranceBack;


	//	Create NPC's and obstacles
	CubicMazeLocation firstKeyLocation = CubicMazeLocation(mazeSize - 1, 0, 0);
	_helper->CreateKeys(&firstKeyLocation, "Key");
	_helper->CreateHazards(&_exitLocation, "Hazard");


	//	Build the scene.
	//BuildScene();

	_levelState = TurboGameLevelState::Running;
}

void Level01::Update(NavigationInfo* navInfo)
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

void Level01::BuildScene(NavigationInfo* navInfo)
{
	int keyCount = _mazeOptions->KeyCount()->GetValue();
	int required = _mazeOptions->RequiredKeyCount()->GetValue();
	uint found = keyCount - _keys.size();
    _mazeOptions->FoundKeyCount()->SetValue(found);
	bool exitLocked = found < required;

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->RightWall.Type = CubicMazeCellWallType::ExitLocked;
		_maze->Cell(_exitLocation)->RightWall.PortalIndex = 0;
	}
	else
	{
		_maze->Cell(_exitLocation)->RightWall.Type = CubicMazeCellWallType::Exit;
		_maze->Cell(_exitLocation)->RightWall.PortalIndex = 1;
	}

	_scene = _helper->BuildScene(navInfo);

	_helper->ClearSignage();
	if (keyCount == 0)
	{
		_helper->AddSignage(0, 0, 0, "Find the exit.");
		_helper->AddSignage(4, 0, -4, "You found the exit.\n\nA blue portal like this one\nis the exit from the maze.\n\nWhen you exit the maze,\nthe next maze will be unlocked.");
	}
	else if (keyCount == 1)
	{
		if (exitLocked)
		{
			_helper->AddSignage(4, 0, -4, "The exit is locked.\n\nYou must find\nthe key\nto unlock it.");
		}
		else
		{
			_helper->AddSignage(4, 0, -4, "The exit is unlocked.\n\nYou must have\nfound the key.\n\nGood job!");
		}
	}

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->RightWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Locked")));
	}
	else
	{
		_maze->Cell(_exitLocation)->RightWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Exit")));
	}

	_sceneChanged = true;
}

//  Local Methods ------------------------------------------------------------------------------------------------------
