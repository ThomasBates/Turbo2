#pragma once

#include <pch.h>

#include <ITurboApplicationPlatform.h>
#include <ITurboSceneObject.h>
#include <ITurboSceneObjectMesh.h>
#include <ITurboSceneObjectPlacement.h>

class SpaceLabyrinthPlayer : public ITurboSceneObject
{
private:
	std::shared_ptr<ITurboApplicationPlatform>	_platform;
	std::shared_ptr<ITurboSceneObjectMesh>		_mesh;
	std::shared_ptr<ITurboSceneObjectPlacement>	_placement;
	std::shared_ptr<ITurboSceneObjectPlacement>	_newPlacement;

	NavigationInfo _lastNavInfo;

public:
	SpaceLabyrinthPlayer(std::shared_ptr<ITurboApplicationPlatform> platform);

	//  ITurboSceneObject Properties
	virtual	std::shared_ptr<ITurboSceneObjectMesh>		Mesh() { return _mesh; }
	virtual	std::shared_ptr<ITurboSceneObjectPlacement>	Placement() { return _placement; }
	virtual	std::shared_ptr<ITurboSceneObjectPlacement>	NewPlacement() { return _newPlacement; }
	virtual	void NewPlacement(std::shared_ptr<ITurboSceneObjectPlacement> newPlacement) { _newPlacement = newPlacement; }

	//  ITurboSceneObject Methods
	virtual void Navigate();
	virtual void Update();
	virtual void Render();

	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, float radius, Vector3D *contact, Vector3D *normal) { return false; };
};