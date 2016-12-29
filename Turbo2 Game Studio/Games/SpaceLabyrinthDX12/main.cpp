#include "pch.h"

#include "ApplicationDX12.h"
#include "TurboApplicationDX12Platform.h"
#include "SpaceLabyrinth.h"

using namespace Application_DX12;

//  The main function is only used to initialize our IFrameworkView class.
//  Edit: The main function calls application->Run, which initializes our
//        IFrameworkViewSource class, which creates our IFrameworkView class,
//        which handles all the windows events.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	std::shared_ptr<IApplication> application = std::shared_ptr<IApplication>(new ApplicationDX12());
	std::shared_ptr<ITurboApplicationPlatform> platform = std::shared_ptr<ITurboApplicationPlatform>(new TurboApplicationDX12Platform(application));
	std::shared_ptr<IProgram> program = std::shared_ptr<IProgram>(new SpaceLabyrinth(platform));

	int result = application->Run(program);

	return result;
}
