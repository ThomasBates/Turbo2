
#include <pch.h>

#include <TurboScene.h>

using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboScene Properties ---------------------------------------------------------------------------------------------

//  ITurboScene Properties ---------------------------------------------------------------------------------------------
//  ITurboScene Methods ------------------------------------------------------------------------------------------------

void TurboScene::AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	_sceneObjects.push_back(sceneObject);
}

void TurboScene::AddSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
	_sceneSprites.push_back(sceneSprite);
}

void TurboScene::AddSceneText(std::shared_ptr<ITurboSceneText> sceneText)
{
	_sceneTexts.push_back(sceneText);
}

//  ITurboScene Methods ------------------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

//  Local Support Methods ----------------------------------------------------------------------------------------------
