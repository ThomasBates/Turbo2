
#include "pch.h"

#include <TurboSceneObject.h>
#include <TurboScenePlacement.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

TurboSceneObject::TurboSceneObject()
{
	_placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
}

TurboSceneObject::TurboSceneObject(std::shared_ptr<ITurboSceneMesh> mesh)
{
	_mesh = mesh;
	_placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------

//void TurboSceneObject::Update()
//{
//
//}

//void TurboSceneObject::Render()
//{
//
//}

bool TurboSceneObject::IsTouching(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal)
{
	return false;
}

//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------
