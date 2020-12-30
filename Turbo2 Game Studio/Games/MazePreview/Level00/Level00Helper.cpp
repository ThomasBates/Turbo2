
#include <pch.h>

#include <Level00Helper.h>

#include <ICubicMazeSceneBuilder.h>
#include <CubicMazeCornerMesh.h>
#include <CubicMazeFactory.h>
#include <CubicMazeMotionEffects_WithoutGravity.h>
#include <CubicMazeObjectInteractions.h>
#include <CubicMazeSceneObject.h>
#include <CubicMazeSignMesh.h>

#include <TurboGameState.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneSound.h>
#include <TurboSceneSprite.h>

using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

//	Constructors and Destructors ---------------------------------------------------------------------------------------
//  Public Methods -----------------------------------------------------------------------------------------------------

void Level00Helper::CreateKeys(CubicMazeLocation* firstKeyLocation, std::string keyTextureName)
{
	if (_keys == NULL)
	{
		return;
	}

	_keys->clear();
	for (int i = 0; i < _mazeOptions->KeyCount; i++)
	{
		std::shared_ptr<ITurboSceneObject> key = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(
			std::shared_ptr<ITurboSceneMesh>(new CubicMazeCornerMesh()),
			std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(keyTextureName))));
		_keys->push_back(key);

		if (i == 0)
		{
			key->Placement()->Move(Position(firstKeyLocation));
		}
		else
		{
			key->Placement()->Move(RandomPosition());
		}

		key->Placement()->AngularVelocity(RandomAngularVelocity());

		if (_mazeOptions->MovingKeys)
		{
			key->Placement()->Velocity(RandomDirection());
		}
	}
}

void Level00Helper::CreateHazards(CubicMazeLocation* firstHazardLocation, std::string hazardTextureName)
{
	if (_hazards == NULL)
	{
		return;
	}

	_hazards->clear();
	for (int i = 0; i < _mazeOptions->HazardCount; i++)
	{
		std::shared_ptr<ITurboSceneObject> hazard = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(
			std::shared_ptr<ITurboSceneMesh>(new CubicMazeCornerMesh()),
			std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(hazardTextureName))));
		_hazards->push_back(hazard);

		if (i == 0)
		{
			hazard->Placement()->Move(Position(firstHazardLocation));
		}
		else
		{
			hazard->Placement()->Move(RandomPosition());
		}

		hazard->Placement()->AngularVelocity(RandomAngularVelocity());

		if (_mazeOptions->MovingHazards)
		{
			hazard->Placement()->Velocity(RandomDirection());
		}
	}
}

std::shared_ptr<ITurboScene> Level00Helper::BuildScene(NavigationInfo* navInfo)
{

	std::shared_ptr<ITurboScene> scene = _sceneBuilder->BuildScene(_maze);
	scene->AddSceneObject(_player);

	//	set player Placement as camera Placement.
	scene->CameraPlacement(_player->Placement());

	//	This is easier for now.
	scene->LightHack(!_mazeOptions->LightsOn);

	if (_keys != NULL)
	{
		for (auto& key : *_keys)
		{
			scene->AddSceneObject(key);
		}
	}

	if (_hazards != NULL)
	{
		for (auto& hazard : *_hazards)
		{
			scene->AddSceneObject(hazard);
		}
	}

	return scene;
}

void Level00Helper::CreateSign(std::shared_ptr<ITurboScene> scene, CubicMazeLocation signLocation, CubicMazeCellWallSide wallSide, std::string signTextureName)
{
	std::shared_ptr<ITurboSceneMesh> signMesh = std::shared_ptr<ITurboSceneMesh>(new CubicMazeSignMesh());
	std::shared_ptr<ITurboSceneMaterial> signMaterial = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(signTextureName));
	std::shared_ptr<ITurboSceneObject> signObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(signMesh, signMaterial));

	switch (wallSide)
	{
	case CubicMazeCellWallSide::Right:
		signObject->Placement()->Rotate(0.0, 0.0, 0.0);
		signObject->Placement()->Move(
			+signLocation.W * CELLSIZE + CELLHALF,
			-signLocation.H * CELLSIZE,
			-signLocation.D * CELLSIZE);
		break;

	case CubicMazeCellWallSide::Left:
		signObject->Placement()->Rotate(0.0, 180.0, 0.0);
		signObject->Placement()->Move(
			+signLocation.W * CELLSIZE - CELLHALF,
			-signLocation.H * CELLSIZE,
			-signLocation.D * CELLSIZE);
		break;

	case CubicMazeCellWallSide::Top:
		signObject->Placement()->Rotate(0.0, 0.0, 90.0);
		signObject->Placement()->Move(
			+signLocation.W * CELLSIZE,
			-signLocation.H * CELLSIZE + CELLHALF,
			-signLocation.D * CELLSIZE);
		break;

	case CubicMazeCellWallSide::Bottom:
		signObject->Placement()->Rotate(0.0, 0.0, -90.0);
		signObject->Placement()->Move(
			+signLocation.W * CELLSIZE,
			-signLocation.H * CELLSIZE - CELLHALF,
			-signLocation.D * CELLSIZE);
		break;

	case CubicMazeCellWallSide::Back:
		signObject->Placement()->Rotate(0.0, -90.0, 0.0);
		signObject->Placement()->Move(
			+signLocation.W * CELLSIZE,
			-signLocation.H * CELLSIZE,
			-signLocation.D * CELLSIZE + CELLHALF);
		break;

	case CubicMazeCellWallSide::Front:
		signObject->Placement()->Rotate(0.0, 90.0, 0.0);
		signObject->Placement()->Move(
			+signLocation.W * CELLSIZE,
			-signLocation.H * CELLSIZE,
			-signLocation.D * CELLSIZE - CELLHALF);
		break;
	}

	scene->AddSceneObject(signObject);

}

bool Level00Helper::Update(NavigationInfo* navInfo, TurboGameLevelState* levelState)
{
	bool rebuildScene = false;

	NavigationInfo npcNavInfo;
	npcNavInfo.Time = navInfo->Time;
	npcNavInfo.DeltaTime = navInfo->DeltaTime;

	//  Update player
	if (*levelState == TurboGameLevelState::Failing)
	{
		_player->Update(navInfo);
		_motionEffects->ProcessMotionEffects(navInfo, _player, true);

		UpdateKeys(&npcNavInfo, "Key", false);
		UpdateHazards(&npcNavInfo, "Fail", false);

		if (navInfo->Time > _failTime + 1.0)
		{
			*levelState = TurboGameLevelState::Failed;
		}

		return rebuildScene;
	}

	_player->Update(navInfo);
	_motionEffects->ProcessMotionEffects(navInfo, _player, true);

	//	Just a precaution in case the player leaves the boundary of the maze.
	//	If so, force a restart of the maze.
	TurboVector3D position = _player->Placement()->Position();
	CubicMazeLocation size = _maze->Size();
	if ((position.X < -CELLHALF + WALLHALF) ||
		(position.X > size.W * CELLSIZE - CELLHALF - WALLHALF) ||
		(position.Y > CELLHALF - WALLHALF) ||
		(position.Y < size.H * -CELLSIZE + CELLHALF + WALLHALF) ||
		(position.Z > CELLHALF - WALLHALF) ||
		(position.Z < size.D * -CELLSIZE + CELLHALF + WALLHALF))
	{
		//	TODO: Place player in the middle of the nearest valid cell.
		*levelState = TurboGameLevelState::Failed;
		return rebuildScene;
	}

	//  Update NPC's and obstacles
	bool hit = UpdateKeys(&npcNavInfo, "Key", true);
	if (hit)
	{
		rebuildScene = true;
	}

	hit = UpdateHazards(&npcNavInfo, "Fail", true);
	if (hit)
	{
		*levelState = TurboGameLevelState::Failing;
		_failTime = navInfo->Time;
	}

	//  Check for collisions
	int portalIndex = 0;
	_objectInteractions->ProcessObjectInteractions(navInfo, _maze, _player, true, &portalIndex);

	if (portalIndex == 1)
	{
		*levelState = TurboGameLevelState::Completed;
	}

	return rebuildScene;
}

bool Level00Helper::UpdateKeys(NavigationInfo* navInfo, std::string keyHitSoundName, bool doHitTest)
{
	if (_keys == NULL)
	{
		return false;
	}

	TurboVector3D playerPosition = _player->Placement()->Position();
	int portalIndex;

	for (auto keyIterator = _keys->begin(); keyIterator != _keys->end(); )
	{
		auto key = *keyIterator;

		key->Placement()->Rotate(key->Placement()->AngularVelocity() * navInfo->DeltaTime);

		if (_mazeOptions->MovingKeys)
		{
			key->Placement()->Velocity(key->Placement()->Velocity().Normalize());

			_motionEffects->ProcessMotionEffects(navInfo, key, false);

			_objectInteractions->ProcessKeyHazardInteractions(navInfo, key, _keys, _hazards);

			_objectInteractions->ProcessObjectInteractions(navInfo, _maze, key, false, &portalIndex);
		}

		if (doHitTest)
		{
			TurboVector3D keyPosition = key->Placement()->Position();
			if ((keyPosition - playerPosition).Length() < 0.25)
			{
				_player->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound(keyHitSoundName)));
				_player->PlaySound(1);

				_keys->erase(keyIterator);
				return true;
			}
		}

		keyIterator++;
	}

	return false;
}

bool Level00Helper::UpdateHazards(NavigationInfo* navInfo, std::string hazardHitSoundName, bool doHitTest)
{
	if (_hazards == NULL)
	{
		return false;
	}

	TurboVector3D playerPosition = _player->Placement()->Position();
	int portalIndex;

	for (auto& hazard : *_hazards)
	{
		hazard->Placement()->Rotate(hazard->Placement()->AngularVelocity() * navInfo->DeltaTime);

		if (_mazeOptions->MovingHazards)
		{
			hazard->Placement()->Velocity(hazard->Placement()->Velocity().Normalize());

			_motionEffects->ProcessMotionEffects(navInfo, hazard, false);

			_objectInteractions->ProcessKeyHazardInteractions(navInfo, hazard, _keys, _hazards);

			_objectInteractions->ProcessObjectInteractions(navInfo, _maze, hazard, false, &portalIndex);
		}

		if (doHitTest)
		{
			TurboVector3D hazardPosition = hazard->Placement()->Position();
			if ((hazardPosition - playerPosition).Length() < 0.25)
			{
				_player->HitSound(std::shared_ptr<ITurboSceneSound>(new TurboSceneSound(hazardHitSoundName)));
				_player->PlaySound(1);

				return true;
			}
		}
	}

	return false;
}

//  Public Methods -----------------------------------------------------------------------------------------------------
//  Private Methods ----------------------------------------------------------------------------------------------------

TurboVector3D Level00Helper::Position(CubicMazeLocation* location)
{
	return TurboVector3D(
		location->W * 2.0,
		location->H * -2.0,
		location->D * -2.0);
}

TurboVector3D Level00Helper::RandomPosition()
{
	CubicMazeLocation size = _maze->Size();
	return TurboVector3D(
		(rand() % size.W) * 2.0,
		(rand() % size.H) * -2.0,
		(rand() % size.D) * -2.0);
}

TurboVector3D Level00Helper::RandomDirection()
{
	return TurboVector3D(
		rand() * 2.0 / RAND_MAX - 1.0,
		rand() * 2.0 / RAND_MAX - 1.0,
		rand() * 2.0 / RAND_MAX - 1.0).Normalize();
}

TurboVector3D Level00Helper::RandomAngularVelocity()
{
	return TurboVector3D(
		rand() % 60 - 30,
		rand() % 60 - 30,
		rand() % 60 - 30);
}

//  Private Methods ----------------------------------------------------------------------------------------------------
