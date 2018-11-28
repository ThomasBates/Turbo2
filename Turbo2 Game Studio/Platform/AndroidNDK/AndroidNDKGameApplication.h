
#pragma once

//  From AndroidNDKGameEngine  -----------------------------------------------------
#include <jni.h>
#include <errno.h>

#include <vector>
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
//  From AndroidNDKGameEngine  -----------------------------------------------------

#include <ITurboGameRenderer.h>
//#include "GLContext.h"
#include <AndroidNDKRenderer.h>


//#include <ITurboDebug.h>
//#include <ITurboGame.h>
#include <ITurboGameApplication.h>
#include <android_native_app_glue.h>
//#include <ITurboGameIOService.h>
//#include <ITurboGameRenderer.h>
//#include <ITurboGameAudio.h>

//using namespace Turbo::Core::Debug;
using namespace Turbo::Game;

namespace Turbo
{
	namespace Platform
	{
		namespace AndroidNDK
		{
			class AndroidNDKGameApplication : public ITurboGameApplication
			{
			public:
				//	Constructors ---------------------------------------------------------------------------------------
				AndroidNDKGameApplication(
                    android_app* app,
					//std::shared_ptr<ITurboDebug> debug,
					//std::shared_ptr<ITurboGameIOService> ioService,
					std::shared_ptr<ITurboGameRenderer> renderer
					//std::shared_ptr<ITurboGameAudio> audio
                        );
				virtual ~AndroidNDKGameApplication() {}

				//  ITurboGame Methods ---------------------------------------------------------------------------------
				virtual int Run(std::shared_ptr<ITurboGame> game);

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                static void HandleCmd(struct android_app* app, int32_t cmd);
                static int32_t HandleInput(android_app* app, AInputEvent* event);
                //  From AndroidNDKGameEngine  -----------------------------------------------------

            private:
                android_app* _android_app;
                //std::shared_ptr<AndroidNDKGameEngine> _engine;

//				std::shared_ptr<ITurboDebug> _debug;
//				std::shared_ptr<ITurboGameIOService> _ioService;
				std::shared_ptr<ITurboGameRenderer> _renderer;
//				std::shared_ptr<ITurboGameAudio> _audio;

                void InitState();

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                AndroidNDKRenderer renderer_;

                ndk_helper::GLContext* _gl_context;

                bool _running = true;
                bool _resources_initialized = false;
                bool _has_focus = false;

                ndk_helper::DoubletapDetector _doubletap_detector;
                ndk_helper::PinchDetector _pinch_detector;
                ndk_helper::DragDetector _drag_detector;
                ndk_helper::PerfMonitor _performance_monitor;
                ndk_helper::TapCamera _tap_camera;

                ASensorManager* _sensor_manager = NULL;
                const ASensor* _accelerometer_sensor = NULL;
                ASensorEventQueue* _sensor_event_queue = NULL;


                bool ProcessEvents();

                void UpdateFPS(float fps);
                void ShowUI();
                void TransformPosition(ndk_helper::Vec2& vec);
                //  From AndroidNDKGameEngine  -----------------------------------------------------

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                int InitDisplay(android_app* app);
                void LoadResources();
                void UnloadResources();
                void DrawFrame();
                void TermDisplay();
                void TrimMemory();
                bool IsReady();

                //void UpdatePosition(AInputEvent* event, int32_t index, float& x, float& y);

                void InitSensors();
                void ProcessSensors(int32_t id);
                void SuspendSensors();
                void ResumeSensors();
                //  From AndroidNDKGameEngine  -----------------------------------------------------

                void InitializeApplication();
            };
		}
	}
}