
#include <pch.h>

#include <TurboScene.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

Turbo::Scene::TurboScene::TurboScene()
{
}

Turbo::Scene::TurboScene::~TurboScene()
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboScene Properties ---------------------------------------------------------------------------------------------

//  ITurboScene Properties ---------------------------------------------------------------------------------------------
//  ITurboScene Methods ------------------------------------------------------------------------------------------------

void Turbo::Scene::TurboScene::AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	_sceneObjects.push_back(sceneObject);
}

void Turbo::Scene::TurboScene::AddSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
	_sceneSprites.push_back(sceneSprite);
}

void Turbo::Scene::TurboScene::AddSceneText(std::shared_ptr<ITurboSceneText> sceneText)
{
	_sceneTexts.push_back(sceneText);
}

//  ITurboScene Methods ------------------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

//  Local Support Methods ----------------------------------------------------------------------------------------------
