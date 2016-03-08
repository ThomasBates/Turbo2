#include "pch.h"

#include "ApplicationDX12.h"

#include "SpaceLabyrinthDX12FrameworkViewSource.h"

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
	_program = program;

	auto viewSource = ref new SpaceLabyrinthDX12FrameworkViewSource();
	CoreApplication::Run(viewSource);
	return 0;
}

#pragma endregion
