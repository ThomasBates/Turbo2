

#pragma once

#include "IComponent.h"
#include "IWindow.h"

class IApplication
{
public:
	virtual int RegisterWindow(IWindow *window) = 0;
	virtual int UnregisterWindow(IWindow *window) = 0;
	virtual int Run() = 0;
};

//extern IApplication *Application;
