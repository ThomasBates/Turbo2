#pragma once

#include <INavigationController.h>
#include <ITurboSceneObjectMesh.h>
#include <ITurboSceneObjectMaterial.h>
#include <ITurboSceneObjectPlacement.h>

class ITurboSceneObject
{
public:
	//  ITurboSceneObject Properties  ----------------------------------------------------------------------------------
	virtual	std::shared_ptr<ITurboSceneObjectMesh>		Mesh() = 0;
	virtual void Mesh(std::shared_ptr<ITurboSceneObjectMesh> mesh) = 0;

	virtual std::shared_ptr<ITurboSceneObjectMaterial>	Material() = 0;
	virtual void Material(std::shared_ptr<ITurboSceneObjectMaterial> material) = 0;

	virtual	std::shared_ptr<ITurboSceneObjectPlacement>	Placement() = 0;
	virtual void Placement(std::shared_ptr<ITurboSceneObjectPlacement> placement) = 0;

	virtual std::vector<std::shared_ptr<ITurboSceneObject>> ChildSceneObjects() = 0;

	//  ITurboSceneObject Methods  -------------------------------------------------------------------------------------
	//virtual void Navigate() = 0;
	virtual void Update(NavigationInfo navInfo) = 0;
	//virtual void Render() = 0;

	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal) = 0;
};

