
#pragma once

#include <JNIHelper.h>
#include <perfMonitor.h>

#include <ITurboGameApplication.h>
#include <ITurboDebug.h>
#include <ITurboViewController.h>
#include <ITurboGameAudio.h>
#include <ITurboGameIOService.h>
#include <ITurboGameRenderer.h>

using namespace Turbo::Core::Debug;
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
					std::shared_ptr<ITurboViewController> controller,
					std::shared_ptr<ITurboGameIOService> ioService,
					std::shared_ptr<ITurboGameRenderer> renderer,
					std::shared_ptr<ITurboGameAudio> audio);
				virtual ~AndroidNDKGameApplication() {}

				//  ITurboGame Methods ---------------------------------------------------------------------------------
				virtual int Run(std::shared_ptr<ITurboGame> game, std::shared_ptr<ITurboView> view);

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                static void HandleAppCmd(struct android_app *app, int32_t cmd);
                //  From AndroidNDKGameEngine  -----------------------------------------------------

            private:
                android_app* _android_app;

				std::shared_ptr<ITurboDebug> _debug;
                std::shared_ptr<ITurboViewController> _controller;
				std::shared_ptr<ITurboGameIOService> _ioService;
				std::shared_ptr<ITurboGameRenderer> _renderer;
				std::shared_ptr<ITurboGameAudio> _audio;

				ndk_helper::PerfMonitor _performance_monitor;

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                bool _hasFocus = false;
                float _width;
                float _height;
				bool _updateControls = false;

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                void InitializeDisplay(android_app *app);
                void ActivateDisplay();
                void DeactivateDisplay();
				void TerminateDisplay();
				void ReconfigureDisplay(android_app *app);
				void UpdateControls(android_app *app);
                void TrimMemory();

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                void ActivityConfigurationChanged(ANativeActivity *activity);

                void JNI_ShowUI();
				void JNI_UpdateFPS();
            };
		}
	}
}