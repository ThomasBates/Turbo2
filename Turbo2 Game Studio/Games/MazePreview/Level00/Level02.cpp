
#include <pch.h>

#include <Level02.h>

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

Level02::Level02(
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
	_mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(CubicMazeType::Layered));
	_objectInteractions = std::shared_ptr<ICubicMazeObjectInteractions>(new CubicMazeObjectInteractions(_debug));
}

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void Level02::Initialize()
{
	//	Create the maze.
	int mazeSize = _mazeOptions->MazeSize()->GetValue();
	_maze = _mazeFactory->MakeMaze(mazeSize, mazeSize, mazeSize);
	_motionEffects = std::shared_ptr<ITurboGameMotionEffects>(new CubicMazeMotionEffects_WithGravity(_maze));

	_helper = std::shared_ptr<Level00Helper>(new Level00Helper(
		_player, _maze, _motionEffects, _sceneBuilder, _objectInteractions,
		_mazeOptions, &_keys, &_hazards));

	//	Create the passages between layers.
	_maze->Cell(2, 0, 0)->BottomWall.Type = CubicMazeCellWallType::None;
	_maze->Cell(2, 1, 0)->TopWall.Type = CubicMazeCellWallType::None;
	_maze->Cell(0, 1, 2)->BottomWall.Type = CubicMazeCellWallType::None;
	_maze->Cell(0, 2, 2)->TopWall.Type = CubicMazeCellWallType::None;

	//	Create the exit.
	_entranceLocation = CubicMazeLocation(0, 0, 2);
	_exitLocation = CubicMazeLocation(2, 2, 0);

	_maze->Cell(_entranceLocation)->LeftWall.Type = EntranceBack;


	//	Create NPC's and obstacles
	CubicMazeLocation firstKeyLocation = CubicMazeLocation(0, mazeSize - 1, 0);
	_helper->CreateKeys(&firstKeyLocation, "Key");
	_helper->CreateHazards(&_exitLocation, "Hazard");


	//	Build the scene.
//	BuildScene();

	_levelState = TurboGameLevelState::Running;
}

void Level02::Update(NavigationInfo* navInfo)
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

void Level02::BuildScene(NavigationInfo* navInfo)
{
	int keyCount = _mazeOptions->KeyCount()->GetValue();
	int required = _mazeOptions->RequiredKeyCount()->GetValue();
    uint found = keyCount - _keys.size();
    _mazeOptions->FoundKeyCount()->SetValue(found);
    bool exitLocked = found < required;

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->BackWall.Type = CubicMazeCellWallType::ExitLocked;
		_maze->Cell(_exitLocation)->BackWall.PortalIndex = 0;
	}
	else
	{
		_maze->Cell(_exitLocation)->BackWall.Type = CubicMazeCellWallType::Exit;
		_maze->Cell(_exitLocation)->BackWall.PortalIndex = 1;
	}

	_scene = _helper->BuildScene(navInfo);

	_helper->ClearSignage();
	if (keyCount == 0)
	{
		_helper->AddSignage(0, 0, -4, "You have to go\ndown two levels\nto find the exit.");
	}

	if (exitLocked)
	{
		_maze->Cell(_exitLocation)->BackWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Locked")));
	}
	else
	{
		_maze->Cell(_exitLocation)->BackWall.SceneObject->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Exit")));
	}

	_sceneChanged = true;
}

//  Local Methods ------------------------------------------------------------------------------------------------------
