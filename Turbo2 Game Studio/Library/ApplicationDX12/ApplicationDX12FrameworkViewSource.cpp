#include "pch.h"

#include "IGameLevel.h"
#include "ApplicationDX12FrameworkView.h"
#include "ApplicationDX12FrameworkViewSource.h"

using namespace Application_DX12;

using namespace Windows::ApplicationModel::Core;

ApplicationDX12FrameworkViewSource::ApplicationDX12FrameworkViewSource()
{
}

ApplicationDX12FrameworkViewSource::ApplicationDX12FrameworkViewSource(std::shared_ptr<IGameLevel> program) :
	_program(program)
{
}

IFrameworkView^ ApplicationDX12FrameworkViewSource::CreateView()
{
	return ref new ApplicationDX12FrameworkView(_program);
}
