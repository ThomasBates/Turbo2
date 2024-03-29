
#pragma once

#include <ITurboDebug.h>
#include <ITurboGame.h>
#include <ITurboGameApplication.h>
#include <ITurboGameIOService.h>
#include <ITurboGameRenderer.h>
#include <ITurboGameAudio.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;

namespace Turbo
{
	namespace Platform
	{
		namespace Windows10
		{
			class Windows10GameApplication : public ITurboGameApplication
			{
			public:
				//	Constructors ---------------------------------------------------------------------------------------
				Windows10GameApplication(
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboGameIOService> ioService,
					std::shared_ptr<ITurboGameRenderer> renderer,
					std::shared_ptr<ITurboGameAudio> audio);

				//  ITurboGame Methods ---------------------------------------------------------------------------------
				virtual int Run(std::shared_ptr<ITurboGame> game);

			private:
				std::shared_ptr<ITurboDebug> _debug;
				std::shared_ptr<ITurboGameIOService> _ioService;
				std::shared_ptr<ITurboGameRenderer> _renderer;
				std::shared_ptr<ITurboGameAudio> _audio;

			};
		}
	}
}