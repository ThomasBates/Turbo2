#pragma once

#include <ITurboSceneObjectMesh.h>
#include <ITurboSceneObjectPlacement.h>

class ITurboSceneObject
{
public:
	//  ITurboSceneObject Properties
	virtual	std::shared_ptr<ITurboSceneObjectMesh>		Mesh() = 0;
	virtual	std::shared_ptr<ITurboSceneObjectPlacement>	Placement() = 0;
	virtual	std::shared_ptr<ITurboSceneObjectPlacement>	NewPlacement() = 0;
	virtual	void NewPlacement(std::shared_ptr<ITurboSceneObjectPlacement> newPlacement) = 0;

	//  ITurboSceneObject Methods
	virtual void Navigate() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, float radius, Vector3D *contact, Vector3D *normal) = 0;
};

