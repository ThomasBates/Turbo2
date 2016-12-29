#pragma once

#include <ITurboSceneObject.h>

class TurboSceneObject : public ITurboSceneObject
{
private:
	std::shared_ptr<ITurboSceneObjectMesh>		_mesh;
	//std::shared_ptr<ITurboSceneObjectPlacement>	_placement;

protected:
	virtual void Mesh(std::shared_ptr<ITurboSceneObjectMesh> mesh) { _mesh = mesh; };

public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	TurboSceneObject();
	TurboSceneObject(std::shared_ptr<ITurboSceneObjectMesh> mesh);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------
	virtual	std::shared_ptr<ITurboSceneObjectMesh>		Mesh() { return _mesh; }
	virtual	std::shared_ptr<ITurboSceneObjectPlacement>	Placement() { return nullptr; }
	virtual	std::shared_ptr<ITurboSceneObjectPlacement>	NewPlacement() { return nullptr; }
	virtual	void NewPlacement(std::shared_ptr<ITurboSceneObjectPlacement> newPlacement) { }

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------
	virtual void Navigate() {};
	virtual void Update();
	virtual void Render();

	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, float radius, Vector3D *contact, Vector3D *normal);
};

