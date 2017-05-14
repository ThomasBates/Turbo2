#pragma once

#include <ITurboSceneObject.h>

class TurboSceneObject : public ITurboSceneObject
{
private:
	std::shared_ptr<ITurboSceneMesh>				_mesh;
	std::shared_ptr<ITurboSceneMaterial>			_material;
	std::shared_ptr<ITurboScenePlacement>			_placement;
	std::vector<std::shared_ptr<ITurboSceneObject>>	_childSceneObjects;

public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	TurboSceneObject();
	TurboSceneObject(std::shared_ptr<ITurboSceneMesh> mesh);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------
	virtual	std::shared_ptr<ITurboSceneMesh> Mesh() { return _mesh; }
	virtual void Mesh(std::shared_ptr<ITurboSceneMesh> mesh) { _mesh = mesh; }

	virtual std::shared_ptr<ITurboSceneMaterial> Material() { return _material; }
	virtual void Material(std::shared_ptr<ITurboSceneMaterial> material) { _material = material; }

	virtual	std::shared_ptr<ITurboScenePlacement> Placement() { return _placement; }
	virtual void Placement(std::shared_ptr<ITurboScenePlacement> placement) { _placement = placement; }

	virtual std::vector<std::shared_ptr<ITurboSceneObject>> ChildSceneObjects() { return _childSceneObjects; }

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------
	virtual void Update(NavigationInfo navInfo) {}
	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal);
};

