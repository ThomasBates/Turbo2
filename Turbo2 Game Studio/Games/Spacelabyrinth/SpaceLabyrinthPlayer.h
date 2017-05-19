#pragma once

#include <pch.h>

#include <ITurboSceneObject.h>

class SpaceLabyrinthPlayer : public ITurboSceneObject
{
private:
	std::shared_ptr<ITurboSceneMesh>				_mesh;
	std::shared_ptr<ITurboScenePlacement>			_placement;
	std::vector<std::shared_ptr<ITurboSceneObject>> _childSceneObjects;

	NavigationInfo _lastNavInfo;

public:
	SpaceLabyrinthPlayer();

	//  ITurboSceneObject Properties -----------------------------------------------------------------------------------
	virtual	std::shared_ptr<ITurboSceneMesh> Mesh() { return _mesh; }
	virtual void Mesh(std::shared_ptr<ITurboSceneMesh> mesh) { _mesh = mesh; }

	virtual std::shared_ptr<ITurboSceneMaterial> Material() { return nullptr; }
	virtual void Material(std::shared_ptr<ITurboSceneMaterial> material) { }

	virtual	std::shared_ptr<ITurboScenePlacement> Placement() { return _placement; }
	virtual void Placement(std::shared_ptr<ITurboScenePlacement> placement) { _placement = placement; }

	virtual std::vector<std::shared_ptr<ITurboSceneObject>> ChildSceneObjects() { return _childSceneObjects; };

	//  ITurboSceneObject Methods --------------------------------------------------------------------------------------
	virtual void Update(NavigationInfo navInfo);
	virtual bool IsTouching(TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *contact, TurboVector3D *normal) { return false; };
};