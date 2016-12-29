
#pragma once

#include "pch.h"

#include "ITurboScene.h"
#include "ITurboApplicationPlatform.h"

class SpaceLabyrinthOriginalScene : public ITurboScene
{
private:
	std::shared_ptr<ITurboApplicationPlatform>	_platform;

	std::vector<std::shared_ptr<ITurboSceneObject>> _sceneObjects;

public:
	//  Constructors and Destructors
	SpaceLabyrinthOriginalScene(std::shared_ptr<ITurboApplicationPlatform> platform);
	~SpaceLabyrinthOriginalScene();

	//  ITurboScene Properties
	virtual std::vector<std::shared_ptr<ITurboSceneObject>> SceneObjects() { return _sceneObjects; }

	//  ITurboScene Methods
	virtual void AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
	virtual int Render();
};

