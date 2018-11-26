//  ============================================================================
//  TestGame.cpp
//  ============================================================================

#include <pch.h>

#include <TestGame.h>
#include <Level00Player.h>

#include <TurboGameState.h>
#include <TurboScene.h>
#include <TurboSceneFont.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneObject.h>
#include <TurboScenePlacement.h>
#include <TurboSceneSprite.h>
#include <TurboSceneText.h>
#include <TurboSceneTexture.h>


//  Constructors and Destructors ---------------------------------------------------------------------------------------

TestGame::TestGame(std::shared_ptr<ITurboDebug> debug) :
	_debug(debug)
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> TestGame::GameState()
{
	std::shared_ptr<ITurboGameState> gameState = nullptr;

	//if (_level != nullptr)
	//{
	//	gameState = _level->GameState();
	//}

	//if (gameState == nullptr)
	//{
	//	gameState = _gameState;
	//}

	//if (gameState == nullptr)
	//{
	//	gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());
	//}

	//gameState->SaveString("TestGame.ProgramInfo", "project info");

	//gameState->SaveBoolean("User.InvertedMouse", _userOptions.InvertedMouse);
	//gameState->SaveBoolean("User.SoundEffectsOn", _userOptions.SoundEffectsOn);

	return gameState;
}

void TestGame::GameState(std::shared_ptr<ITurboGameState> gameState)
{
	//_gameState = gameState;

	//if (gameState == nullptr)
	//{
	//	return;
	//}

	//gameState->LoadString("TestGame.ProgramInfo", "");

	//_userOptions.InvertedMouse = gameState->LoadBoolean("User.InvertedMouse", false);
	//_userOptions.SoundEffectsOn = gameState->LoadBoolean("User.SoundEffectsOn", true);


	//if (_level != nullptr)
	//{
	//	_level->GameState(gameState);
	//}
}

std::shared_ptr<ITurboScene> TestGame::Scene()
{
	//if (_level == nullptr)
	//{
	//	return nullptr;
	//}

	//return _level->Scene();
	return _scene;
}

std::shared_ptr<ITurboSceneObject> TestGame::Player()
{
	//return _player;
	return nullptr;
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void TestGame::Initialize()
{
	//_player = std::shared_ptr<ITurboSceneObject>(new Level00Player(&_userOptions));
}

void TestGame::Finalize()
{
	//if (_level != nullptr)
	//{
	//	_level->Finalize();
	//	_level = nullptr;
	//}
}

void TestGame::Update(NavigationInfo navInfo)
{
	//  Beef this up with multiple levels and state logic.
	_sceneChanged = false;

	//if (navInfo.Restart || (_level == nullptr))
	//{
	//	if (_level != nullptr)
	//	{
	//		_level->Finalize();
	//		_level = nullptr;
	//	}

	//	//_userOptions.InvertedMouse = !_userOptions.InvertedMouse;
	//	_level = std::unique_ptr<ITurboGameLevel>(new Level00(_debug, _player, &_userOptions));
	//	_level->GameState(_gameState);
	//	_level->Initialize();
	//	_sceneChanged = true;
	//}

	//_level->Update(navInfo);

	//if (_level->SceneChanged())
	//{
	//	_sceneChanged = true;
	//}

	if (_scene == nullptr)
	{
		ComposeTestScene();
		_sceneChanged = true;
	}

	if (_rotatingSceneObject != nullptr)
	{
		_rotatingSceneObject->Placement()->Rotate(_rotatingSceneObject->Placement()->AngularVelocity() * navInfo.DeltaTime);
	}
}

void TestGame::ComposeTestScene()
{
	_scene = std::shared_ptr<ITurboScene>(new TurboScene());

	std::shared_ptr<ITurboSceneSprite> sunsetSprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite());
	sunsetSprite->Texture(std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("_sunset")));
	//  default: use rectangle (0,0) to (1,1). (full screen)
	_scene->AddSceneSprite(sunsetSprite);

	std::shared_ptr<ITurboSceneSprite> catSprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite());
	catSprite->Texture(std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("_cat")));
	catSprite->UseRectangle(false);
	catSprite->Location(TurboVector2D(0.1f, 0.8f));
	catSprite->Origin(TurboVector2D(0.5f, 0.5f));
	catSprite->Scale(2.f);
	_scene->AddSceneSprite(catSprite);

	std::shared_ptr<ITurboSceneText> text = std::shared_ptr<ITurboSceneText>(new TurboSceneText());
	text->Font(std::shared_ptr<ITurboSceneFont>(new TurboSceneFont("CourierNew_32")));
	text->Text("Hello Kitty!");
	text->Location(TurboVector2D(0.5f, 0.9f));
	text->Origin(TurboVector2D(0.5f, 0.5f));
	text->BorderColor(TurboColor(0.f, 1.f, 0.f));
	text->BorderSize(0.5f);
	_scene->AddSceneText(text);

	text = std::shared_ptr<ITurboSceneText>(new TurboSceneText());
	text->Font(std::shared_ptr<ITurboSceneFont>(new TurboSceneFont("SegoeUI_16_B")));
	text->Text("Look Up Here!");
	text->Location(TurboVector2D(0.1f, 0.05f));
	text->Origin(TurboVector2D(0.5f, 0.5f));
	text->Color(TurboColor(1.f, 1.f, 0.f));
	text->BorderSize(1.0);
	text->BorderColor(TurboColor(1.f, 0.f, 1.f));
	text->Rotation(0.f);
	_scene->AddSceneText(text);

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new TurboSceneObject());
	sceneObject->PrimitiveType(SceneObjectPrimitiveType::Sphere);
	sceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("_earth")));
	sceneObject->Placement()->GoTo(TurboVector3D(1.0, 0.0, -1.0));
	_scene->AddSceneObject(sceneObject);


	_rotatingSceneObject = std::shared_ptr<ITurboSceneObject>(new TurboSceneObject());
	_rotatingSceneObject->PrimitiveType(SceneObjectPrimitiveType::Sphere);
	_rotatingSceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("_earth")));
	_rotatingSceneObject->Placement()->AngularVelocity(TurboVector3D(0.0, 10.0, 0.0));
	_scene->AddSceneObject(_rotatingSceneObject);

	std::shared_ptr<ITurboScenePlacement> cameraPlacement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
	cameraPlacement->GoTo(2.0, 2.0, 2.0);
	cameraPlacement->RotateY(45.0);
	cameraPlacement->RotateX(-35.0);
	_scene->CameraPlacement(cameraPlacement);
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
