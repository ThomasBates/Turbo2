
#include "pch.h"

#include <TurboSceneObject.h>
#include <TurboScenePlacement.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

Turbo::Scene::TurboSceneObject::TurboSceneObject()
{
	_placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
}

Turbo::Scene::TurboSceneObject::TurboSceneObject(std::shared_ptr<ITurboSceneMesh> mesh)
{
	_mesh = mesh;
	_placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneObject Methods ------------------------------------------------------------------------------------------

//void Turbo::Scene::TurboSceneObject::Update()
//{
//
//}

//void Turbo::Scene::TurboSceneObject::Render()
//{
//
//}

bool Turbo::Scene::TurboSceneObject::IsTouching(TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *contact, TurboVector3D *normal)
{
	return false;
}

//  ITurboSceneObject Methods ------------------------------------------------------------------------------------------
