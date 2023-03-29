
#include <pch.h>

#include <AndroidNDKGameApplication.h>
#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;
using namespace Turbo::Platform::AndroidNDK;

static AndroidNDKGameApplication *_application;

#define HELPER_CLASS_NAME "ca/turbobutterfly/ndkhelper/NDKHelper"  // Class name of helper function
#define ever ;;

//	Constructors -------------------------------------------------------------------------------------------------------

AndroidNDKGameApplication::AndroidNDKGameApplication(
    android_app* app,
	std::shared_ptr<ITurboDebug> debug,
    std::shared_ptr<ITurboViewController> controller,
	std::shared_ptr<ITurboGameIOService> ioService,
	std::shared_ptr<ITurboGameRenderer> renderer
        ) :
    _android_app(app),
	_debug(debug),
    _controller(controller),
	_ioService(ioService),
	_renderer(renderer)
{
    _debug->Send(debugDebug, debugApplication) << "--> AndroidNDKGameApplication()\n";

    // Init helper functions
    //  JNIHelper is a singleton
    ndk_helper::JNIHelper::GetInstance()->Init(_android_app->activity, HELPER_CLASS_NAME);

    _application = this;
    _android_app->onAppCmd = AndroidNDKGameApplication::HandleAppCmd;

    //_android_app->activity->callbacks->onConfigurationChanged = ActivityConfigurationChanged;

    _debug->Send(debugDebug, debugApplication) << "<-- AndroidNDKGameApplication()\n";
}

//  ITurboGameApplication Methods --------------------------------------------------------------------------------------

int AndroidNDKGameApplication::Run(std::shared_ptr<ITurboGame> game, std::shared_ptr<ITurboView> view)
{
    _debug->Send(debugDebug, debugApplication) << "--> AndroidNDKGameApplication::Run()\n";

//    auto gameState = _ioService->LoadGameState();
//    game->GameState(gameState);

    game->Initialize();	//	Create level, create & draw static scene

    // loop waiting for stuff to do.
    for(ever)
    {
        _debug->Send(debugVerbose, debugApplication) << "AndroidNDKGameApplication::Run: _controller->GetNavigationInfo();\n";
        NavigationInfo* navInfo = _controller->GetNavigationInfo();
        _debug->Send(debugVerbose, debugApplication) << "AndroidNDKGameApplication::Run: navInfo->Terminate = " << navInfo->Terminate << "\n";

        if (navInfo->Terminate)
        {
            _debug->Send(debugVerbose, debugApplication) << "AndroidNDKGameApplication::Run: TerminateDisplay();\n";
            TerminateDisplay();
            _debug->Send(debugVerbose, debugApplication) << "<-- AndroidNDKGameApplication::Run(): 0\n";
            return 0;
        }

        _debug->Send(debugVerbose, debugApplication) << "AndroidNDKGameApplication::Run: _hasFocus = " << _hasFocus << "\n";

        if (_hasFocus)
        {
            //	Update the scene
            game->Update(navInfo);

            double currentTime = _performance_monitor.GetCurrentTime();
            if (_updateViewSize > 0 && currentTime >= _updateViewSize)
            {
                _width = (float) ANativeWindow_getWidth(_android_app->window);
                _height = (float) ANativeWindow_getHeight(_android_app->window);

                _updateViewSize = -1;
                view->Size(TurboVector2D(_width, _height));
            }

            _renderer->InitializeLoading();
            view->Load();
            _renderer->FinalizeLoading();

            _renderer->InitializeRendering();
            view->Render();
            _renderer->FinalizeRendering();

            //JNI_UpdateFPS();
        }

        if (_saveGameState)
        {
            _saveGameState = false;
//            gameState = game->GameState();
//            _ioService->SaveGameState(gameState);
        }
    }
}

//  Application Command Methods ----------------------------------------------------------------------------------------

//  Process the next main command.
void AndroidNDKGameApplication::HandleAppCmd(struct android_app *app, int32_t cmd)
{
    _application->HandleCommand(app, cmd);
}

//  Process the next main command.
void AndroidNDKGameApplication::HandleCommand(struct android_app *app, int32_t cmd)
{
    _debug->Send(debugDebug, debugApplication) << "AndroidNDKGameApplication::HandleCommand(app, " << AppCmd(cmd) << ")\n";

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
            SaveGameState(app);
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

    _renderer->Reset();
    UpdateViewSize(app, 0.0f);
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
    UpdateViewSize(app, 0.1f);   //  add a little bit so Android has time to get its story straight.
    JNI_ShowUI();
}

void AndroidNDKGameApplication::TrimMemory()
{
    LOGI("Trimming memory");
    _renderer->Reset();
}

void AndroidNDKGameApplication::UpdateViewSize(android_app *app, double delay)
{
    _updateViewSize = _performance_monitor.GetCurrentTime() + delay;
}

void AndroidNDKGameApplication::SaveGameState(android_app *app)
{
    _saveGameState = true;
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
