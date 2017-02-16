#pragma once

#include "ITurboScene.h"

class ITurboSceneBuilder
{
public:
	//  ITurboSceneBuilder Methods  ------------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene() = 0;
};