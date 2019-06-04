//  ============================================================================
//  TeapotGame.cpp
//  ============================================================================

#include <pch.h>

#include <TurboSceneObject.h>
#include <TurboScene.h>
#include <TurboSceneMaterial.h>

#include <TeapotGame.h>
#include <TeapotMesh.h>
#include <TeapotPlayer.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

TeapotGame::TeapotGame()
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> TeapotGame::GameState()
{
	return nullptr;
}

void TeapotGame::GameState(std::shared_ptr<ITurboGameState> gameState)
{
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void TeapotGame::Initialize()
{
	_scene = std::shared_ptr<ITurboScene>(new TurboScene());

	std::shared_ptr<ITurboSceneMesh> teapotMesh = std::shared_ptr<ITurboSceneMesh>(new TeapotMesh());

	// Init Projection matrices
	teapot_x_ = 8;//numX;
	teapot_y_ = 8;//numY;
	teapot_z_ = 8;//numZ;

	const float total_width = 500.f;
	float gap_x = total_width / (teapot_x_ - 1);
	float gap_y = total_width / (teapot_y_ - 1);
	float gap_z = total_width / (teapot_z_ - 1);
	float offset_x = -total_width / 2.f;
	float offset_y = -total_width / 2.f;
	float offset_z = -total_width / 2.f;

	for (int32_t x = 0; x < teapot_x_; ++x)
	for (int32_t y = 0; y < teapot_y_; ++y)
	for (int32_t z = 0; z < teapot_z_; ++z)
	{
		std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new TurboSceneObject(teapotMesh));

		sceneObject->Placement()->Move(	x * gap_x + offset_x,
										y * gap_y + offset_y,
										z * gap_z + offset_z);

		std::shared_ptr<ITurboSceneMaterial> material = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(""));
		material->SpecularColor(TurboColor(1.0f, 1.0f, 1.0f));
		material->SpecularExponent(10.0f);
		material->AmbientColor(TurboColor(0.1f, 0.1f, 0.1f));
		material->DiffuseColor(TurboColor(
				random() / float(RAND_MAX * 1.1),
				random() / float(RAND_MAX * 1.1),
				random() / float(RAND_MAX * 1.1)));

		sceneObject->Material(material);

		float rotation_x = random() / float(RAND_MAX) - 0.5f;
		float rotation_y = random() / float(RAND_MAX) - 0.5f;
		sceneObject->Placement()->AngularVelocity(TurboVector3D(rotation_x * 180.0f, rotation_y * 180.0f, 0.0f));

		_scene->AddSceneObject(sceneObject);
	}

    _player = std::shared_ptr<ITurboSceneObject>(new TeapotPlayer()); // (&_userOptions));
	_player->Placement()->GoTo(0.0, 0.0, 2000.0);
	_scene->AddSceneObject(_player);
	_scene->CameraPlacement(_player->Placement());

	_sceneChanged = true;
}

void TeapotGame::Update(NavigationInfo* navInfo)
{
	_sceneChanged = false;

	for (auto& sceneObject : _scene->SceneObjects())
	{
        sceneObject->Update(navInfo);

        double deltaTime = navInfo->DeltaTime;
        std::shared_ptr<ITurboScenePlacement> placement = sceneObject->Placement();

        TurboVector3D velocity = placement->Velocity();
        placement->Move(velocity * deltaTime);

        TurboVector3D angularVelocity = placement->AngularVelocity();
        placement->Rotate(angularVelocity * deltaTime);
	}
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
