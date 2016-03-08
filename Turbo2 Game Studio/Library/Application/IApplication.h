

#pragma once

#include "IProgram.h"

class IApplication
{
public:
	virtual int Run(IProgram *program) = 0;
};

//extern IApplication *Application;
