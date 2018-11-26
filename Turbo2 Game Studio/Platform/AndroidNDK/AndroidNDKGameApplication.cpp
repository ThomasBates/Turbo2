
#include <pch.h>

#include <AndroidNDKGameApplication.h>
#include <AndroidNDKGameEngine.h>

//using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Platform::AndroidNDK;

//-------------------------------------------------------------------------
// Preprocessor
//-------------------------------------------------------------------------
#define HELPER_CLASS_NAME "ca/turbobutterfly/ndkhelper/NDKHelper"  // Class name of helper function
//#define HELPER_CLASS_NAME "com/sample/helper/NDKHelper"  // Class name of helper function


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
    _engine = std::shared_ptr<AndroidNDKGameEngine>(new AndroidNDKGameEngine(_android_app, _renderer));

    _engine->SetState(_android_app);

    // Init helper functions
    ndk_helper::JNIHelper::GetInstance()->Init(_android_app->activity, HELPER_CLASS_NAME);

    //_android_app->userData = &_engine;
    _android_app->userData = _engine.get(); //???
    _android_app->onAppCmd = AndroidNDKGameEngine::HandleCmd;
    _android_app->onInputEvent = AndroidNDKGameEngine::HandleInput;

#ifdef USE_NDK_PROFILER
    monstartup("libMazePreviewNativeActivity.so");
#endif

    // Prepare to monitor accelerometer
    _engine->InitSensors();

    // loop waiting for stuff to do.
    while (1)
    {
        // Read all pending events.
        int id;
        int events;
        android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(_engine->IsReady() ? 0 : -1, NULL, &events,
                                     (void**)&source)) >= 0)
        {
            // Process this event.
            if (source != NULL) source->process(_android_app, source);

            _engine->ProcessSensors(id);

            // Check if we are exiting.
            if (_android_app->destroyRequested != 0)
            {
                _engine->TermDisplay();
                return 0;
            }
        }

        if (_engine->IsReady())
        {
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            _engine->DrawFrame();
        }
    }
}

#pragma endregion
