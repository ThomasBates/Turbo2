#include <pch.h>

#include <TurboDebug.h>
#include <TurboDebugMemoryLogger.h>

#include <TurboControlViewModel.h>

#include <OpenGLESRenderer.h>

#include <AndroidNDKDebugLogCatLogger.h>
#include <AndroidNDKGameApplication.h>
#include <AndroidNDKViewController.h>
#include <AndroidNDKIOService.h>
#include <AndroidNDKAudio.h>

#include <MazePreview.h>

#include <MazePreviewMainViewModel.h>
#include <MazePreviewMainControlViewModel.h>
#include <MazePreviewMotionControlViewModel.h>
#include <MazePreviewDirectionControlViewModel.h>

#include <MazePreviewMainView.h>
#include <MazePreviewMainControlView.h>
#include <MazePreviewMotionControlView.h>
#include <MazePreviewDirectionControlView.h>
#include <TurboSceneArialFont.h>


using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::AndroidNDK;
using namespace Turbo::Platform::OpenGLES;

// This is the main entry point of a native application that is using
// android_native_app_glue.  It runs in its own thread, with its own
// event loop for receiving input events and doing other things.

void android_main(android_app* app)
{
    //  Platform resources
    auto logger = std::shared_ptr<ITurboDebugLogger>(new AndroidNDKDebugLogCatLogger("Maze Preview"));
    auto debug = std::shared_ptr<ITurboDebug>(new TurboDebug(logger));

//    debug->Severity(TurboDebugSeverity::debugDebug);
//    debug->CategoryEnabled(TurboDebugCategory::debugApplication, true);
//    debug->CategoryEnabled(TurboDebugCategory::debugController, true);
//    debug->CategoryEnabled(TurboDebugCategory::debugIOService, true);

    auto ioService = std::shared_ptr<ITurboGameIOService>(new AndroidNDKIOService(debug));
    auto renderer = std::shared_ptr<ITurboGameRenderer>(new OpenGLESRenderer(app, debug, ioService));
    auto audio = std::shared_ptr<ITurboGameAudio>(new AndroidNDKAudio(debug, ioService));

    renderer->RegisterFont(std::shared_ptr<ITurboSceneFont>(new TurboSceneArialFont()));
//    renderer->RegisterFont(std::shared_ptr<ITurboSceneFont>(new TurboSceneConsolasFont()));

    auto game = std::shared_ptr<ITurboGame>(new MazePreview(debug));
    auto mainViewModel = std::shared_ptr<MazePreviewMainViewModel>(new MazePreviewMainViewModel(game));

    auto rendererAccess = renderer->RendererAccess();
    auto mainView = std::shared_ptr<ITurboGroupView>(new MazePreviewMainView("Main View", rendererAccess, mainViewModel));

    auto controller = std::shared_ptr<ITurboViewController>(new AndroidNDKViewController(app, debug, mainView));
    auto application = std::shared_ptr<ITurboGameApplication>(new AndroidNDKGameApplication(app, debug, controller, ioService, renderer, audio));

    application->Run(game, mainView);
}
