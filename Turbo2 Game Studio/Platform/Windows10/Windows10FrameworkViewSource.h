#pragma once

#include <ITurboGame.h>
#include <ITurboGameIOService.h>
#include <ITurboGameRenderer.h>

using namespace Windows::ApplicationModel::Core;

using namespace Turbo::Game;

namespace Turbo
{
	namespace Platform
	{
		namespace Windows10
		{
			ref class Windows10FrameworkViewSource /*sealed*/ : IFrameworkViewSource
			{
			public:
				//  IFrameworkViewSource Methods -----------------------------------------------------------------------------------
				virtual IFrameworkView^ CreateView();

			internal:
				//  Constructors ---------------------------------------------------------------------------------------------------
				Windows10FrameworkViewSource();
				Windows10FrameworkViewSource(
					std::shared_ptr<ITurboGame> game,
					std::shared_ptr<ITurboGameIOService> ioService,
					std::shared_ptr<ITurboGameRenderer> renderer,
					std::shared_ptr<ITurboGameAudio> audio);

			private:
				std::shared_ptr<ITurboGame> _game;
				std::shared_ptr<ITurboGameIOService> _ioService;
				std::shared_ptr<ITurboGameRenderer> _renderer;
				std::shared_ptr<ITurboGameAudio> _audio;
			};
		}
	}
}
