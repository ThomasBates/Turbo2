
#include <pch.h>

#include <AndroidNDKGameApplication.h>
#include <AndroidNDKGameController_LookAround.h>
#include "AndroidNDKHelpers.h"

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;
using namespace Turbo::Platform::AndroidNDK;


#define HELPER_CLASS_NAME "ca/turbobutterfly/ndkhelper/NDKHelper"  // Class name of helper function


AndroidNDKGameApplication::AndroidNDKGameApplication(
    android_app* app,
	std::shared_ptr<ITurboDebug> debug,
    std::shared_ptr<ITurboGameController> controller,
	std::shared_ptr<ITurboGameIOService> ioService,
	std::shared_ptr<ITurboGameRenderer> renderer,
	std::shared_ptr<ITurboGameAudio> audio
        ) :
    _android_app(app),
	_debug(debug),
    _controller(controller),
	_ioService(ioService),
	_renderer(renderer),
	_audio(audio)
{
    // Init helper functions
    //  JNIHelper is a singleton
    ndk_helper::JNIHelper::GetInstance()->Init(_android_app->activity, HELPER_CLASS_NAME);

    _android_app->application = this;
    _android_app->onAppCmd = AndroidNDKGameApplication::HandleAppCmd;

    //_android_app->activity->callbacks->onConfigurationChanged = ActivityConfigurationChanged;
}

void AndroidNDKGameApplication::ActivityConfigurationChanged(ANativeActivity *activity)
{
//    activity->assetManager;
}

int AndroidNDKGameApplication::Run(std::shared_ptr<ITurboGame> game)
{
    std::shared_ptr<ITurboGameState> gameState = _ioService->LoadGameState();
    game->GameState(gameState);
    game->Initialize();	//	Create level, create & draw static scene

    // loop waiting for stuff to do.
    while (true)
    {
        NavigationInfo* navInfo = _controller->GetNavigationInfo();

        if (navInfo->Terminate)
        {
            TerminateDisplay();
            return 0;
        }

        if (_hasFocus)
        {
            //	Update the scene
            game->Update(navInfo);

            if (game->SceneChanged())
            {
                _renderer->LoadSceneResources(game->Scene());
                _audio->LoadSceneResources(game->Scene());
            }

            _renderer->RenderScene(game->Scene());
            _audio->PlaySounds(game->Scene());
        }
    }
}

/**
 * Process the next main command.
 */
void AndroidNDKGameApplication::HandleAppCmd(struct android_app *app, int32_t cmd)
{
    AndroidNDKGameApplication* application = (AndroidNDKGameApplication*)app->application;
    switch (cmd)
    {
        //  Open/Resume
        case APP_CMD_START:
            break;
        case APP_CMD_RESUME:
            break;
        case APP_CMD_INPUT_CHANGED:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            application->InitializeDisplay(app);
            break;
        case APP_CMD_GAINED_FOCUS:
            application->_hasFocus = true;
            application->_controller->Resume();
            break;

        //  Pause/Close
        case APP_CMD_PAUSE:
            break;
        case APP_CMD_LOST_FOCUS:
            application->_hasFocus = false;
            application->_controller->Suspend();
            break;
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            application->TerminateDisplay();
            break;

        //  Rotate/Resize
        case APP_CMD_CONFIG_CHANGED:
            application->ReconfigureDisplay(app);
            break;
        case APP_CMD_WINDOW_RESIZED:
            application->ReconfigureDisplay(app);
            break;

        //  Other
        case APP_CMD_WINDOW_REDRAW_NEEDED:
            break;
        case APP_CMD_CONTENT_RECT_CHANGED:
            break;
        case APP_CMD_LOW_MEMORY:
            // Free up GL resources
            application->TrimMemory();
            break;
        case APP_CMD_DESTROY:
            break;

        default:
            break;
    }
}

/**
 * Initialize an EGL context for the current display.
 */
void AndroidNDKGameApplication::InitializeDisplay(android_app *app)
{
    if (app->window != NULL)
    {
        _renderer->UpdateDisplayInformation();
        _controller->Resume();
        _hasFocus = true;
    }
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void AndroidNDKGameApplication::TerminateDisplay()
{
    _hasFocus = false;
    _controller->Suspend();
    _renderer->Reset();
}

void AndroidNDKGameApplication::ReconfigureDisplay(android_app *app)
{
    if (app->window != NULL)
    {
        _renderer->Reset();
        _renderer->UpdateDisplayInformation();
    }
}

void AndroidNDKGameApplication::TrimMemory()
{
    LOGI("Trimming memory");
   // _renderer->Reset();
}
