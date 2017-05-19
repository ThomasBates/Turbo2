#include "pch.h"

#include "DX12Game.h"
#include "DX12Platform.h"
#include "SpaceLabyrinth.h"

using namespace Application_DX12;

//  The main function is only used to initialize our IFrameworkView class.
//  Edit: The main function calls game->Run, which initializes our
//        IFrameworkViewSource class, which creates our IFrameworkView class,
//        which handles all the windows events, which Jack built.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	std::shared_ptr<ITurboGameLevel> mainLevel = std::shared_ptr<ITurboGameLevel>(new SpaceLabyrinth());

	std::shared_ptr<ITurboGame> game = std::shared_ptr<ITurboGame>(new DX12Game());
	
	int result = game->Run(mainLevel);

	return result;
}
