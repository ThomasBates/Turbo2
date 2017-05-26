#include "pch.h"

#include "Windows10FrameworkView.h"
#include "Windows10FrameworkViewSource.h"

using namespace Windows::ApplicationModel::Core;

Turbo::Platform::Windows10::Windows10FrameworkViewSource::Windows10FrameworkViewSource()
{
}

Turbo::Platform::Windows10::Windows10FrameworkViewSource::Windows10FrameworkViewSource(
	std::shared_ptr<ITurboGame> game,
	std::shared_ptr<ITurboGameIOService> ioService,
	std::shared_ptr<ITurboGameRenderer> renderer) :
	_game(game),
	_ioService(ioService),
	_renderer(renderer)
{
}

IFrameworkView^ Turbo::Platform::Windows10::Windows10FrameworkViewSource::CreateView()
{
	return ref new Windows10FrameworkView(_game, _ioService, _renderer);
}
