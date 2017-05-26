
#include "pch.h"

#include "TurboScene.h"

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

//  ITurboScene Methods ------------------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

//  Local Support Methods ----------------------------------------------------------------------------------------------
