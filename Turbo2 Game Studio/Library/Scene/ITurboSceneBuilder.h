#pragma once

#include "ITurboScene.h"

class ITurboSceneBuilder
{
public:
	virtual std::shared_ptr<ITurboScene> BuildScene() = 0;
	virtual void FreeScene(std::shared_ptr<ITurboScene> scene) = 0;
};