
#pragma once

#include <pch.h>
#include <android_native_app_glue.h>
#include <perfMonitor.h>

#include <ITurboViewController.h>
#include <ITurboDebug.h>
#include <ITurboSceneNavigationControl.h>
#include <TurboSceneNavigationInfo.h>
#include <ITurboControlView.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::View;
using namespace Turbo::Scene;

namespace Turbo
{
	namespace Platform
	{
		namespace AndroidNDK
		{
			class AndroidNDKViewController : public ITurboViewController
			{
			public:
				//  Constructors and Destructors -------------------------------------------------------------------------------
				AndroidNDKViewController(
						android_app* app,
						std::shared_ptr<ITurboDebug> debug,
						std::shared_ptr<ITurboView> view);
				virtual ~AndroidNDKViewController() {}

				//  ITurboViewController Methods -------------------------------------------------------------------------------
				virtual NavigationInfo *GetNavigationInfo();
				virtual void Suspend();
				virtual void Resume();

			private:
				android_app* _android_app;
				std::shared_ptr<ITurboDebug> _debug;

				NavigationInfo _navInfo {};
				std::vector<std::shared_ptr<ITurboControlView>> _controlViews;
				bool _isRunning = false;

				ndk_helper::PerfMonitor _performance_monitor;

				std::map<int32_t, std::shared_ptr<ITurboControlView>> _activeControlViews;
				std::map<int32_t, size_t> _activeIndexes;

				static int32_t HandleInputEvents(android_app* app, AInputEvent* event);
				int32_t HandleInputEvent(AInputEvent *event);
				void 	DebugLogMotionEvent(AInputEvent *event);
				int32_t HandleMotionEvent(AInputEvent *event);

				void AddControls(std::shared_ptr<ITurboView> control);
                void AddControl(std::shared_ptr<ITurboControlView> controlView);
                void ClearControls();

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