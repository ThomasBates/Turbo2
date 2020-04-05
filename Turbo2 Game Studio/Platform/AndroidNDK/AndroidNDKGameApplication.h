
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

				//  ITurboGameApplication Methods ----------------------------------------------------------------------
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
				bool _updateViewSize = false;

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                void HandleCommand(struct android_app *app, int32_t cmd);
                void InitializeDisplay(android_app *app);
                void ActivateDisplay();
                void DeactivateDisplay();
				void TerminateDisplay();
				void ReconfigureDisplay(android_app *app);
				void UpdateViewSize(android_app *app);
                void TrimMemory();

                //  From AndroidNDKGameEngine  -----------------------------------------------------
                void ActivityConfigurationChanged(ANativeActivity *activity);

                void JNI_ShowUI();
				void JNI_UpdateFPS();



                inline std::string AppCmd(int32_t cmd)
                {
                    switch (cmd) {
                        case APP_CMD_INPUT_CHANGED:         return "APP_CMD_INPUT_CHANGED";
                        case APP_CMD_INIT_WINDOW:           return "APP_CMD_INIT_WINDOW";
                        case APP_CMD_TERM_WINDOW:           return "APP_CMD_TERM_WINDOW";
                        case APP_CMD_WINDOW_RESIZED:        return "APP_CMD_WINDOW_RESIZED";
                        case APP_CMD_WINDOW_REDRAW_NEEDED:  return "APP_CMD_WINDOW_REDRAW_NEEDED";
                        case APP_CMD_CONTENT_RECT_CHANGED:  return "APP_CMD_CONTENT_RECT_CHANGED";
                        case APP_CMD_GAINED_FOCUS:          return "APP_CMD_GAINED_FOCUS";
                        case APP_CMD_LOST_FOCUS:            return "APP_CMD_LOST_FOCUS";
                        case APP_CMD_CONFIG_CHANGED:        return "APP_CMD_CONFIG_CHANGED";
                        case APP_CMD_LOW_MEMORY:            return "APP_CMD_LOW_MEMORY";
                        case APP_CMD_START:                 return "APP_CMD_START";
                        case APP_CMD_RESUME:                return "APP_CMD_RESUME";
                        case APP_CMD_SAVE_STATE:            return "APP_CMD_SAVE_STATE";
                        case APP_CMD_PAUSE:                 return "APP_CMD_PAUSE";
                        case APP_CMD_STOP:                  return "APP_CMD_STOP";
                        case APP_CMD_DESTROY:               return "APP_CMD_DESTROY";
                    };
                    return "Unknown Application Command";
                }
            };
		}
	}
}