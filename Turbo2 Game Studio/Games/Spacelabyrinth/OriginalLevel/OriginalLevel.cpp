
#include <pch.h>

#include <CubicMazeSceneBuilder.h>
#include <CubicMazeFactory.h>
#include <OriginalLevel.h>
#include <OriginalPlayer.h>
#include <TurboGameState.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

OriginalLevel::OriginalLevel()
{
}

OriginalLevel::~OriginalLevel()
{
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> OriginalLevel::State()
{
	std::shared_ptr<ITurboGameState> state = std::shared_ptr<ITurboGameState>(new TurboGameState());
	state->SaveString("LevelInfo", "level info");
	return state;
}

void OriginalLevel::State(std::shared_ptr<ITurboGameState> state)
{
	state->LoadString("LevelInfo");
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void OriginalLevel::Initialize()
{
	//	Create the maze.
	std::shared_ptr<ICubicMazeFactory> mazeFactory = std::shared_ptr<ICubicMazeFactory>(new CubicMazeFactory(Cube));
	std::shared_ptr<CubicMaze> cubicMaze = mazeFactory->MakeMaze(3, 3, 3);

	//	Create the exit.
	cubicMaze->Cell(2, 2, 2)->FrontWall.Type = Exit;
	cubicMaze->Cell(2, 2, 2)->FrontWall.PortalIndex = 1;


	//	Build the scene.
	std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder());
	_scene = sceneBuilder->BuildScene(cubicMaze);

	//  Create the player
	_player = std::shared_ptr<ITurboSceneObject>(new OriginalPlayer());
	_player->Placement()->Reset();
	_player->Placement()->GoTo(GetSpawnPoint());

	////	set player Placement as camera Placement.
	_scene->CameraPlacement(_player->Placement());

	//  Create NPC's and obstacles ...
	//  ...

	//LoadLevel();
}

void OriginalLevel::Update(NavigationInfo navInfo)
{
	//  Update player
	_player->Update(navInfo);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	ProcessObjectInteractions(navInfo);

	_scene->CameraPlacement(_player->Placement());
}

void OriginalLevel::Finalize()
{
}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods ----------------------------------------------------------------------------------

TurboVector3D OriginalLevel::GetSpawnPoint()
{
	return TurboVector3D(2.0f, -2.0f, -2.0f);
}

void OriginalLevel::ProcessObjectInteractions(NavigationInfo navInfo)
{
	const double buffer = 0.25;
	const double bounciness = 0.25;

	double deltaTime = navInfo.DeltaTime;

	//  Player-Maze Interactions
	std::shared_ptr<ITurboScenePlacement> placement = _player->Placement();
	TurboVector3D oldPosition = placement->Position();
	TurboVector3D velocity = placement->Velocity();
	TurboVector3D angularVelocity = placement->AngularVelocity();

	TurboVector3D contact;
	TurboVector3D normal;

	TurboVector3D nearestContact;
	TurboVector3D nearestNormal;
	double nearestDistance = 10000.0;

	TurboVector3D newPosition = oldPosition + velocity * deltaTime;

	std::vector<std::shared_ptr<ITurboSceneObject>> sceneObjects = _scene->SceneObjects();

	int sceneObjectsCount = sceneObjects.size();

	for (int objectIndex = 0; objectIndex < sceneObjectsCount; objectIndex++)
	{
		std::shared_ptr<ITurboSceneObject> sceneObject = sceneObjects[objectIndex];

		if (sceneObject->IsTouching(oldPosition, newPosition, buffer, &contact, &normal))
		{
			if ((contact - oldPosition).Length() < nearestDistance)
			{
				nearestContact = contact;
				nearestNormal = normal;
				nearestDistance = (contact - oldPosition).Length();
			}
		}
	}

	if (nearestDistance < 10000.0) 
	{
		TurboVector3D v = velocity * 0.75;
		TurboVector3D n = -nearestNormal * (-nearestNormal * v);
		TurboVector3D t = v - n;
		velocity = t - n * 0.5;

		newPosition = oldPosition + velocity * deltaTime;
	}

	placement->Velocity(velocity);
	placement->GoTo(newPosition);
	placement->Rotate(angularVelocity * deltaTime);
}

//----------------------------------------------------------------------------------------------------------------------
