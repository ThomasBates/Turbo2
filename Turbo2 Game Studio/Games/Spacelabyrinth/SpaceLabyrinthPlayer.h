#pragma once

#include <pch.h>

#include <ITurboSceneObject.h>

class SpaceLabyrinthPlayer : public ITurboSceneObject
{
private:
	std::shared_ptr<ITurboSceneObjectMesh>		_mesh;
	std::shared_ptr<ITurboSceneObjectMaterial>	_material;
	std::shared_ptr<ITurboSceneObjectPlacement>	_placement;

	NavigationInfo _lastNavInfo;

public:
	SpaceLabyrinthPlayer();

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------
	virtual	std::shared_ptr<ITurboSceneObjectMesh>		Mesh() { return _mesh; }
	virtual void Mesh(std::shared_ptr<ITurboSceneObjectMesh> mesh) { _mesh = mesh; }

	virtual	std::shared_ptr<ITurboSceneObjectMaterial>	Material() { return _material; }
	virtual void Material(std::shared_ptr<ITurboSceneObjectMaterial> material) { _material = material; }

	virtual	std::shared_ptr<ITurboSceneObjectPlacement>	Placement() { return _placement; }
	virtual void Placement(std::shared_ptr<ITurboSceneObjectPlacement> placement) { _placement = placement; }

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------
	//virtual void Navigate();
	virtual void Update(NavigationInfo navInfo);
	//virtual void Render();

	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal) { return false; };
};