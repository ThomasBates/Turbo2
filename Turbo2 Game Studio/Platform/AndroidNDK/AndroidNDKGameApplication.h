
#pragma once

//#include <ITurboDebug.h>
//#include <ITurboGame.h>
#include <ITurboGameApplication.h>
#include <android_native_app_glue.h>
#include <AndroidNDKGameEngine.h>
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

			private:
                android_app* _android_app;
                std::shared_ptr<AndroidNDKGameEngine> _engine;

//				std::shared_ptr<ITurboDebug> _debug;
//				std::shared_ptr<ITurboGameIOService> _ioService;
				std::shared_ptr<ITurboGameRenderer> _renderer;
//				std::shared_ptr<ITurboGameAudio> _audio;

			};
		}
	}
}