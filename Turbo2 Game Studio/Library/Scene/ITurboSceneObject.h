#pragma once

#include <INavigationController.h>
#include <ITurboSceneMesh.h>
#include <ITurboSceneMaterial.h>
#include <ITurboScenePlacement.h>

class ITurboSceneObject
{
public:
	//  ITurboSceneObject Properties  ----------------------------------------------------------------------------------
	virtual	std::shared_ptr<ITurboSceneMesh>		Mesh() = 0;
	virtual void Mesh(std::shared_ptr<ITurboSceneMesh> mesh) = 0;

	virtual std::shared_ptr<ITurboSceneMaterial> Material() = 0;
	virtual void Material(std::shared_ptr<ITurboSceneMaterial> material) = 0;

	virtual	std::shared_ptr<ITurboScenePlacement>	Placement() = 0;
	virtual void Placement(std::shared_ptr<ITurboScenePlacement> placement) = 0;

	virtual std::vector<std::shared_ptr<ITurboSceneObject>> ChildSceneObjects() = 0;

	//  ITurboSceneObject Methods  -------------------------------------------------------------------------------------
	virtual void Update(NavigationInfo navInfo) = 0;
	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal) = 0;
};

