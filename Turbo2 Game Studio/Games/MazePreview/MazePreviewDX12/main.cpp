#include <pch.h>

#include <TurboDebug.h>
#include <TurboDebugMemoryLogger.h>
#include <Windows10DebugFileLogger.h>

#include <DirectX12Renderer.h>
#include <XAudio2Audio.h>
#include <XAudio2AudioTest.h>

#include <Windows10GameApplication.h>
#include <Windows10IOService.h>
#include <Windows10Audio.h>

#include <MazePreview.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::DirectX12;
using namespace Turbo::Platform::XAudio2;
using namespace Turbo::Platform::Windows10;

//  The main function is only used to initialize our IFrameworkView class.
//  Edit: The main function calls game->Run, which initializes our
//        IFrameworkViewSource class, which creates our IFrameworkView class,
//        which handles all the windows events, which Jack built.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	std::shared_ptr<ITurboDebugLogger> logger = std::shared_ptr<ITurboDebugLogger>(new Windows10DebugFileLogger());
	std::shared_ptr<ITurboDebug> debug = std::shared_ptr<ITurboDebug>(new TurboDebug(logger));
	
	std::shared_ptr<ITurboGameIOService> ioService = std::shared_ptr<ITurboGameIOService>(new Windows10IOService(debug));
	std::shared_ptr<ITurboGameRenderer> renderer = std::shared_ptr<ITurboGameRenderer>(new DirectX12Renderer(debug, ioService));
	std::shared_ptr<ITurboGameAudio> audio = std::shared_ptr<ITurboGameAudio>(new XAudio2Audio(debug, ioService));
	std::shared_ptr<ITurboGameApplication> application = std::shared_ptr<ITurboGameApplication>(new Windows10GameApplication(debug, ioService, renderer, audio));

	std::shared_ptr<ITurboGame> game = std::shared_ptr<ITurboGame>(new MazePreview(debug));

	return application->Run(game);
}
