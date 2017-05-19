#include "pch.h"

#include "ITurboGameLevel.h"
#include "DX12FrameworkView.h"
#include "DX12FrameworkViewSource.h"

using namespace Application_DX12;

using namespace Windows::ApplicationModel::Core;

DX12FrameworkViewSource::DX12FrameworkViewSource()
{
}

DX12FrameworkViewSource::DX12FrameworkViewSource(std::shared_ptr<ITurboGameLevel> program) :
	_level(program)
{
}

IFrameworkView^ DX12FrameworkViewSource::CreateView()
{
	return ref new DX12FrameworkView(_level);
}
