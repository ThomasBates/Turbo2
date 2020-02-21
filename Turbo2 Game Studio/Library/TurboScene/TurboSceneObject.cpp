
#include <pch.h>

#include <TurboSceneObject.h>
#include <TurboScenePlacement.h>

using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

TurboSceneObject::TurboSceneObject()
{
    _placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
}

TurboSceneObject::TurboSceneObject(std::shared_ptr<ITurboSceneMesh> mesh) :
	_mesh(mesh)
{
	_placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneObject Methods ------------------------------------------------------------------------------------------

bool TurboSceneObject::IsTouching(TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *contact, TurboVector3D *normal)
{
	return false;
}

void TurboSceneObject::PlaySound(float volume)
{
	if (_hitSound != nullptr)
	{
		_hitSound->Volume(volume);
		_hitSound->PlaySound(true);
	}
}

//  ITurboSceneObject Methods ------------------------------------------------------------------------------------------
