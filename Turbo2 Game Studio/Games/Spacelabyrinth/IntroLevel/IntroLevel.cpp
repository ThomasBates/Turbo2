
#include <pch.h>

#include <TurboGameState.h>
#include <IntroLevel.h>
#include <IntroPlayer.h>
#include <IntroMazeSceneBuilder.h>

using namespace Turbo::Game;
using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

IntroLevel::IntroLevel()
{
}

IntroLevel::~IntroLevel()
{
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> IntroLevel::State()
{
	std::shared_ptr<ITurboGameState> state = std::shared_ptr<ITurboGameState>(new TurboGameState());
	state->SaveString("LevelInfo", "level info");
	return state;
}

void IntroLevel::State(std::shared_ptr<ITurboGameState> state)
{
	state->LoadString("LevelInfo");
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void IntroLevel::Initialize()
{
	//	Create the scene
	_sceneBuilder = std::unique_ptr<ITurboSceneBuilder>(new IntroMazeSceneBuilder());
	_scene = _sceneBuilder->BuildScene();

	//  Create the player
	_player = std::shared_ptr<ITurboSceneObject>(new IntroPlayer());
	_player->Placement()->Reset();
	_player->Placement()->GoTo(GetSpawnPoint());

	////	set player Placement as camera Placement.
	_scene->CameraPlacement(_player->Placement());

	//  Create NPC's and obstacles ...
	//  ...

	//LoadLevel();
}

void IntroLevel::Update(NavigationInfo navInfo)
{
	//  Update player
	_player->Update(navInfo);

	//  Update NPC's and obstacles
	//  ...

	//  Check for collisions
	ProcessObjectInteractions(navInfo);

	_scene->CameraPlacement(_player->Placement());
}

void IntroLevel::Finalize()
{
}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods ----------------------------------------------------------------------------------

TurboVector3D IntroLevel::GetSpawnPoint()
{
	return TurboVector3D(10.0f, -2.0f, -8.0f);
}

void IntroLevel::ProcessObjectInteractions(NavigationInfo navInfo)
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
