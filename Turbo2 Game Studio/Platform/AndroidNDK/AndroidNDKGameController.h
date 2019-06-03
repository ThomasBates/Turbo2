
#pragma once

#include <pch.h>

#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <tapCamera.h>

#include <ITurboDebug.h>
#include <ITurboGameController.h>
//#include <AndroidNDKNavigationControl.h>
#include <android_native_app_glue.h>
#include <gestureDetector.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;

namespace Turbo
{
	namespace Platform
	{
		namespace AndroidNDK
		{
			//  The reason that DX12NavigationControl is separate from DX12GameController 
			//	is that DX12GameController inherits from an interface in order to be abstracted, 
			//	and DX12NavigationControl has to be a "ref class" in order to receive windows events. 
			//	Apparently one class can't do both. 
			class AndroidNDKGameController : public ITurboGameController
			{
			public:
				//  Constructors and Destructors -------------------------------------------------------------------------------
				AndroidNDKGameController(
						android_app* app,
						std::shared_ptr<ITurboDebug> debug);
				virtual ~AndroidNDKGameController() {}

				//  ITurboGameController Methods -------------------------------------------------------------------------------
				virtual NavigationInfo* GetNavigationInfo();
				virtual void Suspend();
				virtual void Resume();

				static int32_t HandleInputEvents(android_app* app, AInputEvent* event);

			private:
				android_app* _android_app;
				std::shared_ptr<ITurboDebug> _debug;
				//AndroidNDKNavigationControl^ _navigationControl;

				NavigationInfo _navInfo;
				bool _isRunning = false;
				//Timer _timer;

				ndk_helper::PerfMonitor _performance_monitor;
				ndk_helper::DoubletapDetector _doubletap_detector;
				ndk_helper::PinchDetector _pinch_detector;
				ndk_helper::DragDetector _drag_detector;
				//ndk_helper::TapCamera _tap_camera;

				ASensorManager* _sensor_manager = NULL;
				const ASensor* _accelerometer_sensor = NULL;
				ASensorEventQueue* _sensor_event_queue = NULL;

				void InitSensors();
				bool ProcessEvents();
				void ProcessSensors(int32_t id);
				void SuspendSensors();
				void ResumeSensors();

				void DoubleTap();
				void StartDrag();
				void Drag();
				void EndDrag();
				void StartPinch();
				void Pinch();

				bool IsRunning() { return _isRunning; }
				void IsRunning(bool isRunning) { _isRunning = isRunning; }
				void TransformPosition(ndk_helper::Vec2 &vec);
				//bool ProcessEvents();
            };
		}
	}
}