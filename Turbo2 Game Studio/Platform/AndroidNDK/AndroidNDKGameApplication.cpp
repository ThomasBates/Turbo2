
#include <pch.h>

#include <AndroidNDKGameApplication.h>
//#include <AndroidNDKGameEngine.h>

//using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;
using namespace Turbo::Platform::AndroidNDK;

//-------------------------------------------------------------------------
// Preprocessor
//-------------------------------------------------------------------------
#define HELPER_CLASS_NAME "ca/turbobutterfly/ndkhelper/NDKHelper"  // Class name of helper function
//#define HELPER_CLASS_NAME "com/sample/helper/NDKHelper"  // Class name of helper function

//-------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------
const int32_t NUM_TEAPOTS_X = 8;
const int32_t NUM_TEAPOTS_Y = 8;
const int32_t NUM_TEAPOTS_Z = 8;


#pragma region AndroidNDKGameApplication Methods

AndroidNDKGameApplication::AndroidNDKGameApplication(
    android_app* app,
//	std::shared_ptr<ITurboDebug> debug,
//	std::shared_ptr<ITurboGameIOService> ioService,
	std::shared_ptr<ITurboGameRenderer> renderer
//	std::shared_ptr<ITurboGameAudio> audio
        ) :
    _android_app(app),
//	_debug(debug),
//	_ioService(ioService),
	_renderer(renderer)
//	_audio(audio)
{
}

int AndroidNDKGameApplication::Run(std::shared_ptr<ITurboGame> game)
{
    InitializeApplication();

    // loop waiting for stuff to do.
    while (_running)
    {
        _running = ProcessEvents();

        if (IsReady())
        {
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            DrawFrame();

//            //	Update the scene
//            NavigationInfo navInfo = _controller->GetNavigationInfo();
//            game->Update(navInfo);
//
//            if (game->SceneChanged())
//            {
//                _renderer->LoadSceneResources(game->Scene());
////                _audio->LoadSceneResources(game->Scene());
//            }
//
//            //	Render the Scene
//            _renderer->RenderScene(game->Scene());
//
//            //	Play audio
////            _audio->PlaySounds(game->Scene());


        }
    }

    return 0;
}

void AndroidNDKGameApplication::InitializeApplication()
{
    _gl_context = ndk_helper::GLContext::GetInstance();

    // Init helper functions
    ndk_helper::JNIHelper::GetInstance()->Init(_android_app->activity, HELPER_CLASS_NAME);

    _android_app->userData = this;
    _android_app->onAppCmd = AndroidNDKGameApplication::HandleCmd;
    _android_app->onInputEvent = AndroidNDKGameApplication::HandleInput;

    _doubletap_detector.SetConfiguration(_android_app->config);
    _drag_detector.SetConfiguration(_android_app->config);
    _pinch_detector.SetConfiguration(_android_app->config);

    // Prepare to monitor accelerometer
    InitSensors();
}

bool AndroidNDKGameApplication::ProcessEvents()
{
    // Read all pending events.
    int id;
    int events;
    android_poll_source* source;

    // If not animating, we will block forever waiting for events.
    // If animating, we loop until all events are read, then continue
    // to draw the next frame of animation.
    while ((id = ALooper_pollAll(IsReady() ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
    {
        // Process this event.
        if (source != NULL)
        {
            source->process(_android_app, source);
        }

        ProcessSensors(id);

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
void AndroidNDKGameApplication::DrawFrame()
{
    float fps;
    if (_performance_monitor.Update(fps))
    {
        UpdateFPS(fps);
    }
    double dTime = _performance_monitor.GetCurrentTime();
    renderer_.Update(dTime);

    // Just fill the screen with a color.
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer_.Render();

    // Swap
    if (EGL_SUCCESS != _gl_context->Swap())
    {
        UnloadResources();
        LoadResources();
    }
}

/**
 * Process the next input event.
 */
int32_t AndroidNDKGameApplication::HandleInput(android_app* app, AInputEvent* event)
{
    AndroidNDKGameApplication* eng = (AndroidNDKGameApplication*)app->userData;

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        ndk_helper::GESTURE_STATE doubleTapState =
                eng->_doubletap_detector.Detect(event);
        ndk_helper::GESTURE_STATE dragState = eng->_drag_detector.Detect(event);
        ndk_helper::GESTURE_STATE pinchState = eng->_pinch_detector.Detect(event);

        // Double tap detector has a priority over other detectors
        if (doubleTapState == ndk_helper::GESTURE_STATE_ACTION)
        {
            // Detect double tap
            eng->_tap_camera.Reset(true);
        }
        else
        {
            // Handle drag state
            if (dragState & ndk_helper::GESTURE_STATE_START)
            {
                // Otherwise, start dragging
                ndk_helper::Vec2 v;
                eng->_drag_detector.GetPointer(v);
                eng->TransformPosition(v);
                eng->_tap_camera.BeginDrag(v);
            }
            else if (dragState & ndk_helper::GESTURE_STATE_MOVE)
            {
                ndk_helper::Vec2 v;
                eng->_drag_detector.GetPointer(v);
                eng->TransformPosition(v);
                eng->_tap_camera.Drag(v);
            }
            else if (dragState & ndk_helper::GESTURE_STATE_END)
            {
                eng->_tap_camera.EndDrag();
            }

            // Handle pinch state
            if (pinchState & ndk_helper::GESTURE_STATE_START)
            {
                // Start new pinch
                ndk_helper::Vec2 v1;
                ndk_helper::Vec2 v2;
                eng->_pinch_detector.GetPointers(v1, v2);
                eng->TransformPosition(v1);
                eng->TransformPosition(v2);
                eng->_tap_camera.BeginPinch(v1, v2);
            }
            else if (pinchState & ndk_helper::GESTURE_STATE_MOVE)
            {
                // Multi touch
                // Start new pinch
                ndk_helper::Vec2 v1;
                ndk_helper::Vec2 v2;
                eng->_pinch_detector.GetPointers(v1, v2);
                eng->TransformPosition(v1);
                eng->TransformPosition(v2);
                eng->_tap_camera.Pinch(v1, v2);
            }
        }
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
void AndroidNDKGameApplication::HandleCmd(struct android_app* app, int32_t cmd)
{
    AndroidNDKGameApplication* eng = (AndroidNDKGameApplication*)app->userData;
    switch (cmd)
    {
        default:
            break;
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != NULL)
            {
                eng->InitDisplay(app);
                eng->_has_focus = true;
                eng->DrawFrame();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            eng->TermDisplay();
            eng->_has_focus = false;
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_GAINED_FOCUS:
            eng->ResumeSensors();
            // Start animation
            eng->_has_focus = true;
            break;
        case APP_CMD_LOST_FOCUS:
            eng->SuspendSensors();
            // Also stop animating.
            eng->_has_focus = false;
            eng->DrawFrame();
            break;
        case APP_CMD_LOW_MEMORY:
            // Free up GL resources
            eng->TrimMemory();
            break;
    }
}

#pragma endregion

/**
 * Load resources
 */
void AndroidNDKGameApplication::LoadResources()
{
    renderer_.Init(NUM_TEAPOTS_X, NUM_TEAPOTS_Y, NUM_TEAPOTS_Z);
    renderer_.Bind(&_tap_camera);
}

/**
 * Unload resources
 */
void AndroidNDKGameApplication::UnloadResources()
{
    renderer_.Unload();
}

/**
 * Initialize an EGL context for the current display.
 */
int AndroidNDKGameApplication::InitDisplay(android_app *app)
{
    if (!_resources_initialized)
    {
        _gl_context->Init(_android_app->window);
        LoadResources();
        _resources_initialized = true;
    }
    else if(app->window != _gl_context->GetANativeWindow())
    {
        // Re-initialize ANativeWindow.
        // On some devices, ANativeWindow is re-created when the app is resumed
        assert(_gl_context->GetANativeWindow());
        UnloadResources();
        _gl_context->Invalidate();
        _android_app = app;
        _gl_context->Init(app->window);
        LoadResources();
        _resources_initialized = true;
    }
    else
    {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS == _gl_context->Resume(_android_app->window))
        {
            UnloadResources();
            LoadResources();
        }
        else
        {
            assert(0);
        }
    }

    ShowUI();

    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Note that screen size might have been changed
    glViewport(0, 0, _gl_context->GetScreenWidth(), _gl_context->GetScreenHeight());
    renderer_.UpdateViewport();

    _tap_camera.SetFlip(1.f, -1.f, -1.f);
    _tap_camera.SetPinchTransformFactor(10.f, 10.f, 8.f);

    return 0;
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void AndroidNDKGameApplication::TermDisplay()
{
    _gl_context->Suspend();
}

void AndroidNDKGameApplication::TrimMemory()
{
    LOGI("Trimming memory");
    _gl_context->Invalidate();
}


//-------------------------------------------------------------------------
// Sensor handlers
//-------------------------------------------------------------------------
void AndroidNDKGameApplication::InitSensors()
{
    _sensor_manager = ndk_helper::AcquireASensorManagerInstance(_android_app);
    _accelerometer_sensor = ASensorManager_getDefaultSensor(
            _sensor_manager, ASENSOR_TYPE_ACCELEROMETER);
    _sensor_event_queue = ASensorManager_createEventQueue(
            _sensor_manager, _android_app->looper, LOOPER_ID_USER, NULL, NULL);
}

void AndroidNDKGameApplication::ProcessSensors(int32_t id)
{
    // If a sensor has data, process it now.
    if (id == LOOPER_ID_USER)
    {
        if (_accelerometer_sensor != NULL)
        {
            ASensorEvent event;
            while (ASensorEventQueue_getEvents(_sensor_event_queue, &event, 1) > 0)
            {
            }
        }
    }
}

void AndroidNDKGameApplication::ResumeSensors()
{
    // When our app gains focus, we start monitoring the accelerometer.
    if (_accelerometer_sensor != NULL)
    {
        ASensorEventQueue_enableSensor(_sensor_event_queue, _accelerometer_sensor);
        // We'd like to get 60 events per second (in us).
        ASensorEventQueue_setEventRate(_sensor_event_queue, _accelerometer_sensor,
                                       (1000L / 60) * 1000);
    }
}

void AndroidNDKGameApplication::SuspendSensors()
{
    // When our app loses focus, we stop monitoring the accelerometer.
    // This is to avoid consuming battery while not being used.
    if (_accelerometer_sensor != NULL)
    {
        ASensorEventQueue_disableSensor(_sensor_event_queue, _accelerometer_sensor);
    }
}

//-------------------------------------------------------------------------
// Misc
//-------------------------------------------------------------------------
bool AndroidNDKGameApplication::IsReady()
{
    return _has_focus;
}

void AndroidNDKGameApplication::TransformPosition(ndk_helper::Vec2& vec)
{
    vec = ndk_helper::Vec2(2.0f, 2.0f) * vec /
          ndk_helper::Vec2(_gl_context->GetScreenWidth(),
                           _gl_context->GetScreenHeight()) -
          ndk_helper::Vec2(1.f, 1.f);
}

void AndroidNDKGameApplication::ShowUI()
{
    JNIEnv* jni;
    _android_app->activity->vm->AttachCurrentThread(&jni, NULL);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(_android_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "showUI", "()V");
    jni->CallVoidMethod(_android_app->activity->clazz, methodID);

    _android_app->activity->vm->DetachCurrentThread();
    return;
}

void AndroidNDKGameApplication::UpdateFPS(float fps)
{
    JNIEnv* jni;
    _android_app->activity->vm->AttachCurrentThread(&jni, NULL);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(_android_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "updateFPS", "(F)V");
    jni->CallVoidMethod(_android_app->activity->clazz, methodID, fps);

    _android_app->activity->vm->DetachCurrentThread();
    return;
}
