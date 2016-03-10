
#pragma once

#include <stdio.h>						// Header File For Standard Input/Output ( NEW )
#include <windows.h>					// Header File For Windows

#include "IApplication.h"
#include "IApplicationDX12.h"

class ApplicationDX12 : public IApplication //, public IApplicationDX12
{
private:
	LPCWSTR		_appTitle;		// Application Title
//	IProgram*	_program;

public:
	//  Constructors and Destructors
	ApplicationDX12(LPCWSTR appTitle);		// Application Title
	~ApplicationDX12();

	//  IApplication Methods
	virtual BOOL Run(IProgram *program);

	//  IApplicationDX12 Methods
//	virtual IProgram *GetProgram() { return _program; }
};

extern IApplication *Application;
