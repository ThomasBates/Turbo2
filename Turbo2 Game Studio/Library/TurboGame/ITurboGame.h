

#pragma once

#include "ITurboGameLevel.h"

class ITurboGame
{
public:
	virtual int Run(std::shared_ptr<ITurboGameLevel> level) = 0;
};
