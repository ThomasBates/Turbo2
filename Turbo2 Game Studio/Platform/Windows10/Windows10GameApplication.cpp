#include "pch.h"

#include "Windows10GameApplication.h"
#include "Windows10FrameworkViewSource.h"

using namespace Windows::ApplicationModel::Core;
using namespace Turbo::Game;

#pragma region ITurboGame Methods

Turbo::Platform::Windows10::Windows10GameApplication::Windows10GameApplication(
	std::shared_ptr<ITurboGameIOService> ioService,
	std::shared_ptr<ITurboGameRenderer> renderer) :
	_ioService(ioService),
	_renderer(renderer)
{
}

int Turbo::Platform::Windows10::Windows10GameApplication::Run(std::shared_ptr<ITurboGame> game)
{
	IFrameworkViewSource^ viewSource = ref new Windows10FrameworkViewSource(game, _ioService, _renderer);
	CoreApplication::Run(viewSource);
	return 0;
}

#pragma endregion
