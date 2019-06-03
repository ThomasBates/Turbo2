
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
#include <AndroidGLRenderer.h>


//#include <ITurboDebug.h>
//#include <ITurboGame.h>
#include <ITurboGameApplication.h>
#include <ITurboGameAudio.h>
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
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboGameController> controller,
					std::shared_ptr<ITurboGameIOService> ioService,
					std::shared_ptr<ITurboGameRenderer> renderer,
					std::shared_ptr<ITurboGameAudio> audio);
				virtual ~AndroidNDKGameApplication() {}

				//  ITurboGame Methods ---------------------------------------------------------------------------------
				virtual int Run(std::shared_ptr<ITurboGame> game);

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                static void HandleAppCmd(struct android_app *app, int32_t cmd);
                //  From AndroidNDKGameEngine  -----------------------------------------------------

            private:
                android_app* _android_app;

				std::shared_ptr<ITurboDebug> _debug;
                std::shared_ptr<ITurboGameController> _controller;
				std::shared_ptr<ITurboGameIOService> _ioService;
				std::shared_ptr<ITurboGameRenderer> _renderer;
				std::shared_ptr<ITurboGameAudio> _audio;

                //  From AndroidNDKGameEngine  -----------------------------------------------------

                bool _hasFocus = false;

                ndk_helper::DoubletapDetector _doubletap_detector;
                ndk_helper::PinchDetector _pinch_detector;
                ndk_helper::DragDetector _drag_detector;
//                ndk_helper::TapCamera _tap_camera;
//
//                ASensorManager* _sensor_manager = NULL;
//                const ASensor* _accelerometer_sensor = NULL;
//                ASensorEventQueue* _sensor_event_queue = NULL;


                bool ProcessEvents();
				void DrawFrame(std::shared_ptr<ITurboGame> game);
                //  From AndroidNDKGameEngine  -----------------------------------------------------

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                void InitDisplay(android_app* app);
				void TermDisplay();
//                void LoadResources();
//                void UnloadResources();
				void ResumeSensors();
				void SuspendSensors();
                void TrimMemory();

                bool HasFocus();
                void HasFocus(bool hasFocus);

//                void UpdatePosition(AInputEvent* event, int32_t index, float& x, float& y);
//                void InitSensors();
//                void ProcessSensors(int32_t id);
//                void SuspendSensors();
//                void ResumeSensors();

                //  From AndroidNDKGameEngine  -----------------------------------------------------

            };
		}
	}
}