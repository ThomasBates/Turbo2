#include <pch.h>

#include <DirectX12Renderer.h>
#include <Windows10GameApplication.h>
#include <Windows10IOService.h>
#include <Windows10Platform.h>

#include <SpaceLabyrinth.h>

using namespace Turbo::Platform::DirectX12;
using namespace Turbo::Platform::Windows10;

//  The main function is only used to initialize our IFrameworkView class.
//  Edit: The main function calls game->Run, which initializes our
//        IFrameworkViewSource class, which creates our IFrameworkView class,
//        which handles all the windows events, which Jack built.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	std::shared_ptr<ITurboGameIOService> ioService = std::shared_ptr<ITurboGameIOService>(new Windows10IOService());
	std::shared_ptr<ITurboGameRenderer> renderer = std::shared_ptr<ITurboGameRenderer>(new DirectX12Renderer(ioService));
	std::shared_ptr<ITurboGameApplication> application = std::shared_ptr<ITurboGameApplication>(new Windows10GameApplication(ioService, renderer));

	std::shared_ptr<ITurboGame> game = std::shared_ptr<ITurboGame>(new SpaceLabyrinth());

	return application->Run(game);
}
