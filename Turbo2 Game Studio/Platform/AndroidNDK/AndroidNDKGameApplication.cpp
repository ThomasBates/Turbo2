
#include <pch.h>

#include <AndroidNDKGameApplication.h>
#include <AndroidNDKGameController.h>
#include "AndroidNDKHelpers.h"

//using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;
using namespace Turbo::Platform::AndroidNDK;

//-------------------------------------------------------------------------
// Preprocessor
//-------------------------------------------------------------------------
#define HELPER_CLASS_NAME "ca/turbobutterfly/ndkhelper/NDKHelper"  // Class name of helper function
//#define HELPER_CLASS_NAME "com/sample/helper/NDKHelper"  // Class name of helper function

////-------------------------------------------------------------------------
//// Constants
////-------------------------------------------------------------------------
//const int32_t NUM_TEAPOTS_X = 8;
//const int32_t NUM_TEAPOTS_Y = 8;
//const int32_t NUM_TEAPOTS_Z = 8;


#pragma region AndroidNDKGameApplication Methods

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
            TermDisplay();
            return 0;
        }

        if (HasFocus())
        {
            //	Update the scene
            game->Update(navInfo);

            if (game->SceneChanged())
            {
                _renderer->LoadSceneResources(game->Scene());
                _audio->LoadSceneResources(game->Scene());
            }

            //	Render the Scene
            _renderer->RenderScene(game->Scene());

            //	Play audio
            _audio->PlaySounds(game->Scene());
        }
    }
}

bool AndroidNDKGameApplication::ProcessEvents()
{
    // Read all pending events.
    int id;

    //  If the timeout is zero, returns immediately without blocking.
    //  If the timeout is negative, waits indefinitely until an event appears.
    int timeout = HasFocus() ? 0 : -1;
    int events;
    android_poll_source* source;

    // If not animating, we will block forever waiting for events.
    // If animating, we loop until all events are read, then continue
    // to draw the next frame of animation.
    while ((id = ALooper_pollAll(timeout, NULL, &events, (void**)&source)) >= 0)
    {
        // Process this event.
        if (source != NULL)
        {
            source->process(_android_app, source);
        }

        //ProcessSensors(id);

        // Check if we are exiting.
        if (_android_app->destroyRequested != 0)
        {
            TermDisplay();
            return false;
        }
    }

    return true;
}

/**
 * Just the current frame in the display.
 */
void AndroidNDKGameApplication::DrawFrame(std::shared_ptr<ITurboGame> game)
{
    //	Update the scene
    NavigationInfo* navInfo = _controller->GetNavigationInfo();
    game->Update(navInfo);

    if (game->SceneChanged())
    {
        _renderer->LoadSceneResources(game->Scene());
        _audio->LoadSceneResources(game->Scene());
    }

    //	Render the Scene
    _renderer->RenderScene(game->Scene());

    //	Play audio
    _audio->PlaySounds(game->Scene());
}

/**
 * Process the next main command.
 */
void AndroidNDKGameApplication::HandleAppCmd(struct android_app *app, int32_t cmd)
{
    AndroidNDKGameApplication* application = (AndroidNDKGameApplication*)app->application;
    switch (cmd)
    {
        default:
            break;
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            application->InitDisplay(app);
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            application->TermDisplay();
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_GAINED_FOCUS:
            application->HasFocus(true);
            break;
        case APP_CMD_LOST_FOCUS:
            application->HasFocus(false);
            break;
        case APP_CMD_LOW_MEMORY:
            // Free up GL resources
            application->TrimMemory();
            break;
    }
}

#pragma endregion

/**
 * Load resources
 */
//void AndroidNDKGameApplication::LoadResources()
//{
//    renderer_.Init(NUM_TEAPOTS_X, NUM_TEAPOTS_Y, NUM_TEAPOTS_Z);
//    renderer_.Bind(&_tap_camera);
//}

/**
 * Unload resources
 */
//void AndroidNDKGameApplication::UnloadResources()
//{
//    renderer_.Unload();
//}

/**
 * Initialize an EGL context for the current display.
 */
void AndroidNDKGameApplication::InitDisplay(android_app *app)
{
    if (app->window != NULL)
    {
        _renderer->UpdateDisplayInformation();
        HasFocus(true);
        //DrawFrame(_game);
    }
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void AndroidNDKGameApplication::TermDisplay()
{
    _renderer->Reset();
    //_renderer->Suspend();
    //_gl_context->Suspend();
    HasFocus(false);
}

void AndroidNDKGameApplication::TrimMemory()
{
    LOGI("Trimming memory");
    _renderer->Reset();
    //_gl_context->Invalidate();
}

//-------------------------------------------------------------------------
// Misc
//-------------------------------------------------------------------------
bool AndroidNDKGameApplication::HasFocus()
{
    return _hasFocus;
}

void AndroidNDKGameApplication::HasFocus(bool hasFocus)
{
    _hasFocus = hasFocus;
    if (_hasFocus)
    {
        _controller->Resume();
    }
    else
    {
        _controller->Suspend();
    }
}
