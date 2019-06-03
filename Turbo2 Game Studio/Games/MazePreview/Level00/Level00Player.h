#pragma once

#include <pch.h>

#include <ITurboSceneObject.h>
#include <TurboSceneTypes.h>
#include <Level00Types.h>

using namespace Turbo::Math;
using namespace Turbo::Scene;

class Level00Player : public ITurboSceneObject
{
public:
	Level00Player(Level00UserOptions* userOptions);

	//  ITurboSceneObject Properties -----------------------------------------------------------------------------------
	virtual SceneObjectPrimitiveType PrimitiveType() { return SceneObjectPrimitiveType::None; }
	virtual void PrimitiveType(SceneObjectPrimitiveType primitivetype) {}

	virtual	std::shared_ptr<ITurboSceneMesh> Mesh() { return _mesh; }
	virtual void Mesh(std::shared_ptr<ITurboSceneMesh> mesh) { _mesh = mesh; }

	virtual std::shared_ptr<ITurboSceneMaterial> Material() { return nullptr; }
	virtual void Material(std::shared_ptr<ITurboSceneMaterial> material) { }

	virtual std::shared_ptr<ITurboSceneSoundEffect> HitSound() { return _hitSound; }
	virtual void HitSound(std::shared_ptr<ITurboSceneSoundEffect> hitSound) { _hitSound = hitSound; }

	virtual	std::shared_ptr<ITurboSceneLight> Light() { return _light; }
	virtual void Light(std::shared_ptr<ITurboSceneLight> light) { _light = light; }

	virtual	std::shared_ptr<ITurboScenePlacement> Placement() { return _placement; }
	virtual void Placement(std::shared_ptr<ITurboScenePlacement> placement) { _placement = placement; }

	virtual std::vector<std::shared_ptr<ITurboSceneObject>> ChildSceneObjects() { return _childSceneObjects; };

	//  ITurboSceneObject Methods --------------------------------------------------------------------------------------
	virtual void Update(NavigationInfo* navInfo);
	virtual bool IsTouching(TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *contact, TurboVector3D *normal) { return false; };
	virtual void PlaySound(float volume);

private:
	std::shared_ptr<ITurboSceneMesh>				_mesh;
	std::shared_ptr<ITurboSceneSoundEffect>			_hitSound;
	std::shared_ptr<ITurboScenePlacement>			_placement;
	std::shared_ptr<ITurboSceneLight>				_light;
	std::vector<std::shared_ptr<ITurboSceneObject>> _childSceneObjects;
	Level00UserOptions*								_userOptions;

	NavigationInfo _lastNavInfo;
};