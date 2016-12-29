#pragma once

#include <ITurboSceneObject.h>

class ITurboScene
{
public:
	//  ITurboScene Properties
	virtual std::vector<std::shared_ptr<ITurboSceneObject>> SceneObjects() = 0;

	//  ITurboScene Methods
	virtual void AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject) = 0;
	virtual int Render() = 0;
};
