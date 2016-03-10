#include "pch.h"

#include "ApplicationDX12.h"

#include "ApplicationDX12FrameworkViewSource.h"

using namespace Windows::ApplicationModel::Core;

IApplication *Application;

#pragma region Constructors and Destructors

ApplicationDX12::ApplicationDX12(LPCWSTR appTitle)
{
	Application = this;

	_appTitle	= appTitle;				// Application Title
}

ApplicationDX12::~ApplicationDX12()
{
}

#pragma endregion
#pragma region IApplication Methods

BOOL ApplicationDX12::Run(IProgram *program)
{
//	_program = program;

	IFrameworkViewSource^ viewSource = ref new ApplicationDX12FrameworkViewSource(program);
	CoreApplication::Run(viewSource);
	return 0;
}

#pragma endregion
