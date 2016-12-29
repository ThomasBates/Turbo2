
#include "pch.h"

#include "SpaceLabyrinthOriginalLevel.h"
#include "SpaceLabyrinthPlayer.h"
#include "OriginalMazeSceneBuilder.h"

//============  Constructors and Destructors  --------------------------------------------------------------------------

SpaceLabyrinthOriginalLevel::SpaceLabyrinthOriginalLevel(std::shared_ptr<ITurboApplicationPlatform> platform)
{
	_platform = platform;
}

SpaceLabyrinthOriginalLevel::~SpaceLabyrinthOriginalLevel()
{
	_sceneBuilder->FreeScene(_scene);
}

//----------------------------------------------------------------------------------------------------------------------
//============  IGameLevel Methods  ------------------------------------------------------------------------------------

int SpaceLabyrinthOriginalLevel::Initialize()
{
	int result = _platform->Initialize();

	//	Create the scene
	_sceneBuilder = std::unique_ptr<ITurboSceneBuilder>(new OriginalMazeSceneBuilder(_platform));
	_scene = _sceneBuilder->BuildScene();

	//  Create the player
	_player = std::shared_ptr<ITurboSceneObject>(new SpaceLabyrinthPlayer(_platform));
	_player->Placement()->Reset();
	_player->Placement()->GoTo(GetSpawnPoint());
	//	set player Placement as camera Placement.
	_platform->CameraPlacement(_player->Placement());

	//  Create NPC's and obstacles ...
	//  ...

	RenderStaticScene();

	return result;
}

int SpaceLabyrinthOriginalLevel::Update()
{
	_platform->BeginUpdate();

	UpdateDynamicSceneObjects();

	_platform->EndUpdate();

	return 1;
}

int SpaceLabyrinthOriginalLevel::Render()
{
	_platform->BeginRender();

	RenderDynamicSceneObjects();
	
	_platform->EndRender();

	return 1;
}

int SpaceLabyrinthOriginalLevel::SaveState()
{
	//_platform->SaveState(_state);
	return 1;
}

int SpaceLabyrinthOriginalLevel::Finalize()
{
	return _platform->Finalize();
}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods  ---------------------------------------------------------------------------------

Vector3D SpaceLabyrinthOriginalLevel::GetSpawnPoint()
{
	return Vector3D(2.0f, -2.0f, -2.0f);
}

void SpaceLabyrinthOriginalLevel::RenderStaticScene()
{
	_platform->BeginDraw();
	_scene->Render();
	_platform->EndDraw();
}

void SpaceLabyrinthOriginalLevel::UpdateDynamicSceneObjects()
{
	//  Navigate player
	_player->Navigate();

	//  Navigate NPC's and obstacles
	//  ...

	//  Check for collisions
	ProcessObjectInteractions();

	//  Update player
	_player->Update();

	//  Update NPC's and obstacles
	//  ...

}

void SpaceLabyrinthOriginalLevel::RenderDynamicSceneObjects()
{
	//  Render player
	_player->Render();
	//_platform->RenderSceneObject(_player);

	//  Render NPC's and obstacles
	//  ...

	//_platform->CameraPlacement(_player->Placement());
}

void SpaceLabyrinthOriginalLevel::ProcessObjectInteractions()
{
	const float buffer = 0.25;
	const float bounciness = 0.25;

	NavigationInfo navInfo = _platform->GetNavigationInfo();
	float deltaTime = navInfo.DeltaTime;

	//  Player-Maze Interactions
	std::shared_ptr<ITurboSceneObjectPlacement> placement = _player->Placement();
	Vector3D oldPosition = placement->Position();
	Vector3D velocity = placement->Velocity();
	Vector3D angularVelocity = placement->AngularVelocity();

	Vector3D contact;
	Vector3D normal;

	Vector3D nearestContact;
	Vector3D nearestNormal;
	float nearestDistance = 10000.0;

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
