#include <pch.h>

#include <TurboDebug.h>
#include <TurboDebugMemoryLogger.h>
#include <TurboGameAggregateRenderer.h>
#include <TurboGameNullAudioRenderer.h>

#include <OpenGLESRenderer.h>
#include <OboeAudioRenderer.h>

#include <AndroidNDKConfigProvider.h>
#include <AndroidNDKDebugLogCatLogger.h>
#include <AndroidNDKGameApplication.h>
#include <AndroidNDKViewController.h>
#include <AndroidNDKIOService.h>

#include <MazePreview.h>
#include <MazePreviewGameState.h>

#include <MazePreviewRootViewModel.h>
#include <MazePreviewRootView.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::AndroidNDK;
using namespace Turbo::Platform::OpenGLES;
using namespace Turbo::Platform::Oboe;

// This is the main entry point of a native application that is using
// android_native_app_glue.  It runs in its own thread, with its own
// event loop for receiving input events and doing other things.

void android_main(android_app* app)
{
    //  Platform resources
    auto logger = std::shared_ptr<ITurboDebugLogger>(new AndroidNDKDebugLogCatLogger("Maze Preview"));
    auto debug = std::shared_ptr<ITurboDebug>(new TurboDebug(logger));

//    debug->Severity(TurboDebugSeverity::debugVerbose);
//    debug->CategoryEnabled(TurboDebugCategory::debugAudio, true);

    auto stateProvider = std::shared_ptr<ITurboConfigProvider>(new AndroidNDKConfigProvider(debug, "MazePreview.txt"));
    auto gameState = std::shared_ptr<MazePreviewGameState>(new MazePreviewGameState(debug, stateProvider, "MazePreview"));

    auto ioService = std::shared_ptr<ITurboGameIOService>(new AndroidNDKIOService(debug));

    auto graphicsRenderer = std::shared_ptr<ITurboGameRenderer>(new OpenGLESRenderer(app, debug, ioService));
    auto audioRenderer = std::shared_ptr<ITurboGameRenderer>(new OboeAudioRenderer(debug, ioService));
    //auto audioRenderer = std::shared_ptr<ITurboGameRenderer>(new TurboGameNullAudioRenderer());
    auto renderer = std::shared_ptr<ITurboGameRenderer>(new TurboGameAggregateRenderer({graphicsRenderer, audioRenderer}));

    auto game = std::shared_ptr<ITurboGame>(new MazePreview(debug, gameState));
    auto rootViewModel = std::shared_ptr<MazePreviewRootViewModel>(new MazePreviewRootViewModel(game, gameState));

    auto rendererAccess = renderer->RendererAccess();
    auto rootView = std::shared_ptr<ITurboGroupView>(new MazePreviewRootView(debug, "Root View", rendererAccess, rootViewModel));

    auto controller = std::shared_ptr<ITurboViewController>(new AndroidNDKViewController(app, debug, rootView));
    auto application = std::shared_ptr<ITurboGameApplication>(new AndroidNDKGameApplication(app, debug, controller, ioService, renderer));

    application->Run(game, rootView);
}
