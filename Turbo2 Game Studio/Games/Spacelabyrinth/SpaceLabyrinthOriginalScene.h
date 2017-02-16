
#pragma once

#include "pch.h"

#include "ITurboScene.h"

class SpaceLabyrinthOriginalScene : public ITurboScene
{
private:
	std::vector<std::shared_ptr<ITurboSceneObject>> _sceneObjects;
	std::shared_ptr<ITurboSceneObjectPlacement>		_cameraPlacement;

public:
	//  Constructors and Destructors  ----------------------------------------------------------------------------------
	SpaceLabyrinthOriginalScene();
	~SpaceLabyrinthOriginalScene();

	//  ITurboScene Properties  ----------------------------------------------------------------------------------------
	virtual std::vector<std::shared_ptr<ITurboSceneObject>> SceneObjects() { return _sceneObjects; }

	virtual std::shared_ptr<ITurboSceneObjectPlacement> CameraPlacement() { return _cameraPlacement; }
	virtual void CameraPlacement(std::shared_ptr<ITurboSceneObjectPlacement> cameraPlacement) { _cameraPlacement = cameraPlacement; }

	//  ITurboScene Methods  -------------------------------------------------------------------------------------------
	virtual void AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
	//virtual int Render();
};

