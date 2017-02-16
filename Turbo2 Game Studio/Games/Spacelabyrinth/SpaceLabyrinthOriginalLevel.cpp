
#include "pch.h"

#include <ApplicationState.h>
#include "SpaceLabyrinthOriginalLevel.h"
#include "SpaceLabyrinthPlayer.h"
#include "OriginalMazeSceneBuilder.h"

//  Constructors and Destructors  --------------------------------------------------------------------------------------

SpaceLabyrinthOriginalLevel::SpaceLabyrinthOriginalLevel()
{
}

SpaceLabyrinthOriginalLevel::~SpaceLabyrinthOriginalLevel()
{
}

//  IGameLevel Properties  ---------------------------------------------------------------------------------------------

std::shared_ptr<IApplicationState> SpaceLabyrinthOriginalLevel::State()
{
	std::shared_ptr<IApplicationState> state = std::shared_ptr<IApplicationState>(new ApplicationState());
	state->SaveString("LevelInfo", "level info");
	return state;
}

void SpaceLabyrinthOriginalLevel::State(std::shared_ptr<IApplicationState> state)
{
	state->LoadString("LevelInfo");
}

//  IGameLevel Methods  ------------------------------------------------------------------------------------------------

void SpaceLabyrinthOriginalLevel::Initialize()
{
	//	Create the scene
	_sceneBuilder = std::unique_ptr<ITurboSceneBuilder>(new OriginalMazeSceneBuilder());
	_scene = _sceneBuilder->BuildScene();

	//  Create the player
	_player = std::shared_ptr<ITurboSceneObject>(new SpaceLabyrinthPlayer());
	_player->Placement()->Reset();
	_player->Placement()->GoTo(GetSpawnPoint());

	////	set player Placement as camera Placement.
	_scene->CameraPlacement(_player->Placement());

	//  Create NPC's and obstacles ...
	//  ...

	//RenderStaticScene();
}

void SpaceLabyrinthOriginalLevel::Update(NavigationInfo navInfo)
{
	//  Update player
	_player->Update(navInfo);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	ProcessObjectInteractions(navInfo);

	_scene->CameraPlacement(_player->Placement());
}

void SpaceLabyrinthOriginalLevel::Finalize()
{
}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods  ---------------------------------------------------------------------------------

Vector3D SpaceLabyrinthOriginalLevel::GetSpawnPoint()
{
	return Vector3D(2.0f, -2.0f, -2.0f);
}

void SpaceLabyrinthOriginalLevel::ProcessObjectInteractions(NavigationInfo navInfo)
{
	const double buffer = 0.25;
	const double bounciness = 0.25;

	double deltaTime = navInfo.DeltaTime;

	//  Player-Maze Interactions
	std::shared_ptr<ITurboSceneObjectPlacement> placement = _player->Placement();
	Vector3D oldPosition = placement->Position();
	Vector3D velocity = placement->Velocity();
	Vector3D angularVelocity = placement->AngularVelocity();

	Vector3D contact;
	Vector3D normal;

	Vector3D nearestContact;
	Vector3D nearestNormal;
	double nearestDistance = 10000.0;

	Vector3D newPosition = oldPosition + velocity * deltaTime;

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
		Vector3D v = velocity * bounciness;
		Vector3D n = -nearestNormal * (-nearestNormal * v);
		Vector3D t = v - n;
		velocity = t - n;

		newPosition = oldPosition + velocity * deltaTime;
	}

	placement->Velocity(velocity);
	placement->GoTo(newPosition);
	placement->Rotate(angularVelocity * deltaTime);
}

//----------------------------------------------------------------------------------------------------------------------
