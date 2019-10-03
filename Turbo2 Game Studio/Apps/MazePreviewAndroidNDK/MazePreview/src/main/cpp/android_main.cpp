#include <pch.h>

#include <TurboDebug.h>
#include <TurboDebugMemoryLogger.h>
#include <TurboGameControllerViewModel.h>
#include <AndroidNDKDebugLogCatLogger.h>

#include <OpenGLESRenderer.h>

#include <AndroidNDKGameApplication.h>
#include <AndroidNDKGameController.h>
#include <AndroidNDKIOService.h>
#include <AndroidNDKAudio.h>

#include <MazePreview.h>
#include <TeapotGame.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::AndroidNDK;
using namespace Turbo::Platform::OpenGLES;

// This is the main entry point of a native application that is using
// android_native_app_glue.  It runs in its own thread, with its own
// event loop for receiving input events and doing other things.

void android_main(android_app* app)
{
    std::shared_ptr<ITurboDebugLogger> logger = std::shared_ptr<ITurboDebugLogger>(new AndroidNDKDebugLogCatLogger());
    std::shared_ptr<ITurboDebug> debug = std::shared_ptr<ITurboDebug>(new TurboDebug(logger));

    std::shared_ptr<ITurboGameController> controller = std::shared_ptr<ITurboGameController>(new AndroidNDKGameController(app, debug));
    std::shared_ptr<ITurboGameControllerViewModel> controllerViewModel = std::shared_ptr<ITurboGameControllerViewModel>(new TurboGameControllerViewModel(debug, controller));

    std::shared_ptr<ITurboGameIOService> ioService = std::shared_ptr<ITurboGameIOService>(new AndroidNDKIOService(debug));
    std::shared_ptr<ITurboGameRenderer> renderer = std::shared_ptr<ITurboGameRenderer>(new OpenGLESRenderer(app, debug, ioService, controllerViewModel));
    std::shared_ptr<ITurboGameAudio> audio = std::shared_ptr<ITurboGameAudio>(new AndroidNDKAudio(debug, ioService));

    std::shared_ptr<ITurboGameApplication> application = std::shared_ptr<ITurboGameApplication>(new AndroidNDKGameApplication(app, debug, controller, ioService, renderer, audio));
    //std::shared_ptr<ITurboGame> game = std::shared_ptr<ITurboGame>(new TeapotGame());
    std::shared_ptr<ITurboGame> game = std::shared_ptr<ITurboGame>(new MazePreview(debug));

    application->Run(game);
}
