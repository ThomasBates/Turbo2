
#pragma once

#include <pch.h>

//#include <android/sensor.h>
//#include <android_native_app_glue.h>

#include <ITurboDebug.h>
#include <ITurboSceneNavigationControl.h>
#include <ITurboGameController.h>
#include <TurboSceneNavigationInfo.h>
#include <android_native_app_glue.h>

#include <perfMonitor.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

namespace Turbo
{
	namespace Platform
	{
		namespace AndroidNDK
		{
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

			private:
				android_app* _android_app;
				std::shared_ptr<ITurboDebug> _debug;

				NavigationInfo _navInfo;
				bool _isRunning = false;

				ndk_helper::PerfMonitor _performance_monitor;

				std::map<int32_t, std::shared_ptr<ITurboSceneNavigationControl>> _activeControls;
				std::map<int32_t, size_t> _activeIndexes;

				static int32_t HandleInputEvents(android_app* app, AInputEvent* event);
				int32_t HandleInputEvent(AInputEvent *event);
				void 	DebugLogMotionEvent(AInputEvent *event);
				int32_t HandleMotionEvent(AInputEvent *event);

				void InitializeSensors();
				void InitializeControls();
				bool ProcessEvents();
				void ProcessSensors(int32_t id);
				void SuspendSensors();
				void ResumeSensors();

				void UpdatePointerIndexes(const AInputEvent *event);
				int32_t GetPointerIndex(const AInputEvent *event, int32_t id);
			};
		}
	}
}