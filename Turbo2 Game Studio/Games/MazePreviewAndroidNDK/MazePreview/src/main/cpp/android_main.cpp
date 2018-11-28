#include <pch.h>

//#include <TurboDebug.h>
//#include <TurboDebugMemoryLogger.h>
//#include <AndroidNDKDebugFileLogger.h>

#include <AndroidNDKRenderer.h>

#include <AndroidNDKGameApplication.h>
//#include <AndroidNDKIOService.h>
//#include <AndroidNDKAudio.h>

//#include <MazePreview.h>
#include <TeapotGame.h>

//using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::AndroidNDK;

// This is the main entry point of a native application that is using
// android_native_app_glue.  It runs in its own thread, with its own
// event loop for receiving input events and doing other things.

//  The main function is only used to initialize our IFrameworkView class.
//  Edit: The main function calls game->Run,
//        which initializes our IFrameworkViewSource class,
//        which creates our IFrameworkView class,
//        which handles all the windows events,
//        which Jack built.
void android_main(android_app* app)
{
    std::shared_ptr<ITurboGameRenderer> renderer = std::shared_ptr<ITurboGameRenderer>(new AndroidNDKRenderer(/*debug, ioService*/));
    std::shared_ptr<ITurboGameApplication> application = std::shared_ptr<ITurboGameApplication>(new AndroidNDKGameApplication(app, renderer));

    std::shared_ptr<ITurboGame> game = std::shared_ptr<ITurboGame>(new TeapotGame());

    application->Run(game);
}
