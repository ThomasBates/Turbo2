#include "pch.h"

#include "ApplicationDX12.h"
#include "SpaceLabyrinthDX12Platform.h"
#include "SpaceLabyrinth.h"

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	IApplication *application = new ApplicationDX12(TEXT("Space Labyrinth"));
	ISpaceLabyrinthPlatform *platform = new SpaceLabyrinthDX12Platform(application);
	IProgram *program = new SpaceLabyrinth(platform);

	int result = application->Run(program);

	delete program;
	delete platform;
	delete application;

	return result;
}
