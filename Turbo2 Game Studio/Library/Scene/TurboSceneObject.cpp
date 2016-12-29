
#include "pch.h"

#include <ITurboApplicationPlatform.h>
#include <TurboSceneObject.h>
#include <TurboSceneObjectPlacement.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

TurboSceneObject::TurboSceneObject()
{
	//_mesh = mesh;
	//_placement = std::shared_ptr<ITurboSceneObjectPlacement>(new TurboSceneObjectPlacement());
}

TurboSceneObject::TurboSceneObject(std::shared_ptr<ITurboSceneObjectMesh> mesh)
{
	_mesh = mesh;
	//_placement = std::shared_ptr<ITurboSceneObjectPlacement>(new TurboSceneObjectPlacement());
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------

void TurboSceneObject::Update()
{

}

void TurboSceneObject::Render()
{

}

bool TurboSceneObject::IsTouching(Vector3D oldPosition, Vector3D newPosition, float radius, Vector3D *contact, Vector3D *normal)
{
	return false;
}

//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------
