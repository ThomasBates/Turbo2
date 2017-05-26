#pragma once

#include <pch.h>

#include <ITurboGame.h>
#include <ITurboGameIOService.h>
#include <ITurboGameRenderer.h>

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;

using namespace Turbo::Game;

namespace Turbo
{
	namespace Platform
	{
		namespace Windows10
		{
			// Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
			ref class Windows10FrameworkView sealed : public IFrameworkView
			{
			internal:
				//	Constructors ---------------------------------------------------------------------------------------
				Windows10FrameworkView(
					std::shared_ptr<ITurboGame> game, 
					std::shared_ptr<ITurboGameIOService> ioService,
					std::shared_ptr<ITurboGameRenderer> renderer);

			public:
				//	Constructors ---------------------------------------------------------------------------------------
				Windows10FrameworkView();

				//	IFrameworkView methods -----------------------------------------------------------------------------
				virtual void Initialize(CoreApplicationView^ applicationView);
				virtual void SetWindow(CoreWindow^ window);
				virtual void Load(String^ entryPoint);
				virtual void Run();
				virtual void Uninitialize();

			protected:
				//	Application lifecycle event handlers ---------------------------------------------------------------
				void OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args);
				void OnSuspending(Object^ sender, SuspendingEventArgs^ args);
				void OnResuming(Object^ sender, Object^ args);

				//	Window event handlers ------------------------------------------------------------------------------
				void OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args);
				void OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args);
				void OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args);

				//	DisplayInformation event handlers ------------------------------------------------------------------
				void OnDpiChanged(DisplayInformation^ sender, Object^ args);
				void OnOrientationChanged(DisplayInformation^ sender, Object^ args);
				void OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args);

			private:
				std::shared_ptr<ITurboGame>				_game;
				std::shared_ptr<ITurboGameIOService>	_ioService;
				std::shared_ptr<ITurboGameRenderer>		_renderer;
				std::shared_ptr<ITurboGameController>	_controller;

				bool _windowClosed;
				bool _windowVisible;
			};
		}
	}
}