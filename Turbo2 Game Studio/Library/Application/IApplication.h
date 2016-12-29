

#pragma once

#include "IProgram.h"

class IApplication
{
public:
	virtual int Run(std::shared_ptr<IProgram> program) = 0;
};

//extern IApplication *Application;
