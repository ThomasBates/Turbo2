#include <pch.h>

#include <TurboDebug.h>
#include <TurboDebugMemoryLogger.h>

#include <TurboControlViewModel.h>

#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>

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

    auto ioService = std::shared_ptr<ITurboGameIOService>(new AndroidNDKIOService(debug));
    auto renderer = std::shared_ptr<ITurboGameRenderer>(new OpenGLESRenderer(app, debug, ioService));
    auto audio = std::shared_ptr<ITurboGameAudio>(new AndroidNDKAudio(debug, ioService));

    auto game = std::shared_ptr<ITurboGame>(new MazePreview(debug));

    auto dragControl    = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationPadControl(TurboGameControlType::Look,  0.1F));
    auto dpadControl    = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationPadControl(TurboGameControlType::Look, -0.2F));
    auto forwardButton  = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Move, 0, -1, 0));
    auto backwardButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Move, 0,  1, 0));

    //  View Models
    auto dragControlViewModel    = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(dragControl));
    auto dpadControlViewModel    = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(dpadControl, "DPadControl"));
    auto forwardButtonViewModel  = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(forwardButton, "ForwardButton"));
    auto backwardButtonViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(backwardButton, "BackwardButton"));

    auto sceneViewModel = std::shared_ptr<TurboSceneViewModel>(new TurboSceneViewModel(game));
//    auto hud1ViewModel = std::shared_ptr<MazePreviewHUD1ViewModel>(new MazePreviewHUD1ViewModel(game));
//    auto hud2ViewModel = std::shared_ptr<MazePreviewHUD2ViewModel>(new MazePreviewHUD2ViewModel(game));

    auto mainControlViewModel      = std::shared_ptr<MazePreviewMainControlViewModel>(new MazePreviewMainControlViewModel(dragControlViewModel));
    auto motionControlViewModel    = std::shared_ptr<MazePreviewMotionControlViewModel>(new MazePreviewMotionControlViewModel(forwardButtonViewModel, backwardButtonViewModel));
    auto directionControlViewModel = std::shared_ptr<MazePreviewDirectionControlViewModel>(new MazePreviewDirectionControlViewModel(dpadControlViewModel));

    auto mainViewModel = std::shared_ptr<MazePreviewMainViewModel>(new MazePreviewMainViewModel(
            sceneViewModel,
//            hud1ViewModel,
//            hud2ViewModel,
            mainControlViewModel,
            motionControlViewModel,
            directionControlViewModel
            ));

    auto rendererAccess = std::dynamic_pointer_cast<ITurboViewRendererAccess>(renderer);

    auto mainView = std::shared_ptr<ITurboGroupView>(new MazePreviewMainView("Main View", rendererAccess, mainViewModel));

    auto controller = std::shared_ptr<ITurboViewController>(new AndroidNDKViewController(app, debug, mainView));

    auto application = std::shared_ptr<ITurboGameApplication>(new AndroidNDKGameApplication(app, debug, controller, ioService, renderer, audio));

    application->Run(game, mainView);
}
