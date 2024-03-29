#include <pch.h>

#include <Windows10GameApplication.h>
#include <Windows10FrameworkViewSource.h>

using namespace Windows::ApplicationModel::Core;

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;

#pragma region ITurboGame Methods

Turbo::Platform::Windows10::Windows10GameApplication::Windows10GameApplication(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboGameIOService> ioService,
	std::shared_ptr<ITurboGameRenderer> renderer,
	std::shared_ptr<ITurboGameAudio> audio) :
	_debug(debug),
	_ioService(ioService),
	_renderer(renderer),
	_audio(audio)
{
}

int Turbo::Platform::Windows10::Windows10GameApplication::Run(std::shared_ptr<ITurboGame> game)
{
	IFrameworkViewSource^ viewSource = ref new Windows10FrameworkViewSource(game, _ioService, _renderer, _audio);
	CoreApplication::Run(viewSource);
	return 0;
}

#pragma endregion
