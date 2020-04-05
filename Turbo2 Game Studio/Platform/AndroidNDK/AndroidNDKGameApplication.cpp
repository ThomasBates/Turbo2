
#include <pch.h>

#include <AndroidNDKGameApplication.h>
#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;
using namespace Turbo::Platform::AndroidNDK;


#define HELPER_CLASS_NAME "ca/turbobutterfly/ndkhelper/NDKHelper"  // Class name of helper function
#define ever ;;

//	Constructors -------------------------------------------------------------------------------------------------------

AndroidNDKGameApplication::AndroidNDKGameApplication(
    android_app* app,
	std::shared_ptr<ITurboDebug> debug,
    std::shared_ptr<ITurboViewController> controller,
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
    _debug->Send(debugDebug, debugApplication) << "--> AndroidNDKGameApplication()\n";

    // Init helper functions
    //  JNIHelper is a singleton
    ndk_helper::JNIHelper::GetInstance()->Init(_android_app->activity, HELPER_CLASS_NAME);

    _android_app->application = this;
    _android_app->onAppCmd = AndroidNDKGameApplication::HandleAppCmd;

    //_android_app->activity->callbacks->onConfigurationChanged = ActivityConfigurationChanged;

    _debug->Send(debugDebug, debugApplication) << "<-- AndroidNDKGameApplication()\n";
}

//  ITurboGameApplication Methods --------------------------------------------------------------------------------------

int AndroidNDKGameApplication::Run(std::shared_ptr<ITurboGame> game, std::shared_ptr<ITurboView> view)
{
    _debug->Send(debugDebug, debugApplication) << "--> Run()\n";

    std::shared_ptr<ITurboGameState> gameState = _ioService->LoadGameState();
    game->GameState(gameState);
    game->Initialize();	//	Create level, create & draw static scene

    // loop waiting for stuff to do.
    for(ever)
    {
        _debug->Send(debugVerbose, debugApplication) << "Run: _controller->GetNavigationInfo();\n";
        NavigationInfo* navInfo = _controller->GetNavigationInfo();
        _debug->Send(debugVerbose, debugApplication) << "Run: navInfo->Terminate = " << navInfo->Terminate << "\n";

        if (navInfo->Terminate)
        {
            _debug->Send(debugVerbose, debugApplication) << "Run: TerminateDisplay();\n";
            TerminateDisplay();
            _debug->Send(debugVerbose, debugApplication) << "<-- Run(): 0\n";
            return 0;
        }

        _debug->Send(debugVerbose, debugApplication) << "Run: _hasFocus = " << _hasFocus << "\n";

        if (_hasFocus)
        {
            //	Update the scene
            game->Update(navInfo);

            if (_updateViewSize)
            {
                _updateViewSize = false;
                view->Size(TurboVector2D(_width, _height));
            }

            _renderer->LoadView(view);
            _renderer->RenderView(view);

//            _audio->LoadView(view);
//            _audio->PlaySounds(view);

            JNI_UpdateFPS();
        }
    }
}

//  Application Command Methods ----------------------------------------------------------------------------------------

//  Process the next main command.
void AndroidNDKGameApplication::HandleAppCmd(struct android_app *app, int32_t cmd)
{
    auto application = (AndroidNDKGameApplication*)app->application;
    application->HandleCommand(app, cmd);
}

//  Process the next main command.
void AndroidNDKGameApplication::HandleCommand(struct android_app *app, int32_t cmd)
{
    _debug->Send(debugDebug, debugApplication) << "HandleCommand(app, " << AppCmd(cmd) << ")\n";

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
            InitializeDisplay(app);
            break;
        case APP_CMD_GAINED_FOCUS:
            ActivateDisplay();
            break;

            //  Pause/Close
        case APP_CMD_PAUSE:
            break;
        case APP_CMD_LOST_FOCUS:
            DeactivateDisplay();
            break;
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            TerminateDisplay();
            break;

            //  Rotate/Resize
        case APP_CMD_CONFIG_CHANGED:
            ReconfigureDisplay(app);
            break;
        case APP_CMD_WINDOW_RESIZED:
            ReconfigureDisplay(app);
            break;

            //  Other
        case APP_CMD_WINDOW_REDRAW_NEEDED:
            break;
        case APP_CMD_CONTENT_RECT_CHANGED:
            break;
        case APP_CMD_LOW_MEMORY:
            // Free up GL resources
            TrimMemory();
            break;
        case APP_CMD_DESTROY:
            break;

        default:
            break;
    }
}

void AndroidNDKGameApplication::ActivityConfigurationChanged(ANativeActivity *activity)
{
//    activity->assetManager;
}

//  Initialize an EGL context for the current display.
void AndroidNDKGameApplication::InitializeDisplay(android_app *app)
{
    if (app->window == NULL)
        return;

    _renderer->UpdateDisplayInformation();
    UpdateViewSize(app);
    JNI_ShowUI();

    _controller->Resume();
    //_hasFocus = true;
}

//  Initialize an EGL context for the current display.
void AndroidNDKGameApplication::ActivateDisplay()
{
    _controller->Resume();
    _hasFocus = true;
}

//  Tear down the EGL context currently associated with the display.
void AndroidNDKGameApplication::DeactivateDisplay()
{
    _hasFocus = false;
    _controller->Suspend();
}

//  Tear down the EGL context currently associated with the display.
void AndroidNDKGameApplication::TerminateDisplay()
{
    //_hasFocus = false;
    _controller->Suspend();
    _renderer->Reset();
}

//  Occurs when the device is turned from portrait to landscape or vice versa.
void AndroidNDKGameApplication::ReconfigureDisplay(android_app *app)
{
    if (app->window == NULL)
        return;

    _renderer->Reset();
    _renderer->UpdateDisplayInformation();

    UpdateViewSize(app);

    JNI_ShowUI();
}

void AndroidNDKGameApplication::TrimMemory()
{
    LOGI("Trimming memory");
    _renderer->Reset();
}

void AndroidNDKGameApplication::UpdateViewSize(android_app *app)
{
    _width = (float) ANativeWindow_getWidth(app->window);
    _height = (float) ANativeWindow_getHeight(app->window);

    _updateViewSize = true;
}

//  JNI Methods --------------------------------------------------------------------------------------------------------

void AndroidNDKGameApplication::JNI_ShowUI()
{
    JNIEnv *jni;
    _android_app->activity->vm->AttachCurrentThread(&jni, NULL);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(_android_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "showUI", "()V");
    jni->CallVoidMethod(_android_app->activity->clazz, methodID);

    _android_app->activity->vm->DetachCurrentThread();
}

void AndroidNDKGameApplication::JNI_UpdateFPS()
{
    float fps;
    if (!_performance_monitor.Update(fps))
    {
        return;
    }

    JNIEnv *jni;
    _android_app->activity->vm->AttachCurrentThread(&jni, NULL);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(_android_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "updateFPS", "(F)V");
    jni->CallVoidMethod(_android_app->activity->clazz, methodID, fps);

    _android_app->activity->vm->DetachCurrentThread();
}
