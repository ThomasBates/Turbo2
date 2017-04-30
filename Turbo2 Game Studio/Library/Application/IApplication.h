

#pragma once

#include "IGameLevel.h"

class IApplication
{
public:
	virtual int Run(std::shared_ptr<IGameLevel> level) = 0;
};

//extern IApplication *Application;
