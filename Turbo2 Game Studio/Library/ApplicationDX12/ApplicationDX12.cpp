#include "pch.h"

#include "ApplicationDX12.h"
#include "ApplicationDX12FrameworkViewSource.h"

using namespace Windows::ApplicationModel::Core;
using namespace Application_DX12;

#pragma region IApplication Methods

BOOL ApplicationDX12::Run(IProgram *program)
{
	IFrameworkViewSource^ viewSource = ref new ApplicationDX12FrameworkViewSource(program);
	CoreApplication::Run(viewSource);
	return 0;
}

#pragma endregion
