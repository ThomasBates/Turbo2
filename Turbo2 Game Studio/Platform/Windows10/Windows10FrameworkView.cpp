
#include <pch.h>

#include <Windows10FrameworkView.h>
#include <Windows10GameController.h>

using namespace Concurrency;
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

// The DirectX 12 Application template is documented at http://go.microsoft.com/fwlink/?LinkID=613670&clcid=0x409

Turbo::Platform::Windows10::Windows10FrameworkView::Windows10FrameworkView() :
	_windowClosed(false),
	_windowVisible(true)
{
}

Turbo::Platform::Windows10::Windows10FrameworkView::Windows10FrameworkView(
	std::shared_ptr<ITurboGame> game,
	std::shared_ptr<ITurboGameIOService> ioService,
	std::shared_ptr<ITurboGameRenderer> renderer) :
	_game(game),
	_ioService(ioService),
	_renderer(renderer),
	_windowClosed(false),
	_windowVisible(true)
{
}

// The first method called when the IFrameworkView is being created.
void Turbo::Platform::Windows10::Windows10FrameworkView::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &Windows10FrameworkView::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &Windows10FrameworkView::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Object^>(this, &Windows10FrameworkView::OnResuming);
}

// Called when the CoreWindow object is created (or re-created).
void Turbo::Platform::Windows10::Windows10FrameworkView::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &Windows10FrameworkView::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &Windows10FrameworkView::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &Windows10FrameworkView::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &Windows10FrameworkView::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &Windows10FrameworkView::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &Windows10FrameworkView::OnDisplayContentsInvalidated);
}

// Initializes scene resources, or loads a previously saved app state.
void Turbo::Platform::Windows10::Windows10FrameworkView::Load(String^ entryPoint)
{
	_controller = std::shared_ptr<ITurboGameController>(new Windows10GameController());
	_game->Initialize();	//	Create level, create & draw static scene
}

// This method is called after the window becomes active.
void Turbo::Platform::Windows10::Windows10FrameworkView::Run()
{
	while (!_windowClosed)
	{
		if (_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			//	Update the scene
			NavigationInfo navInfo = _controller->GetNavigationInfo();
			_game->Update(navInfo);
			if (_game->SceneChanged())
			{
				_renderer->LoadSceneResources(_game->Scene());
			}

			//	Render the Scene
			_renderer->RenderScene(_game->Scene());
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void Turbo::Platform::Windows10::Windows10FrameworkView::Uninitialize()
{
}

// Application lifecycle event handlers.

void Turbo::Platform::Windows10::Windows10FrameworkView::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

// Notifies the app that it is being suspended.
void Turbo::Platform::Windows10::Windows10FrameworkView::OnSuspending(Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		// Process lifetime management may terminate suspended apps at any time, so it is
		// good practice to save any state that will allow the app to restart where it left off.

		std::shared_ptr<ITurboGameState> gameState = _game->GameState();
		_ioService->SaveGameState(gameState);

		// If your application uses video memory allocations that are easy to re-create,
		// consider releasing that memory to make it available to other applications.

		deferral->Complete();
	});
}

// Notifes the app that it is no longer suspended.
void Turbo::Platform::Windows10::Windows10FrameworkView::OnResuming(Object^ sender, Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	std::shared_ptr<ITurboGameState> gameState = _ioService->LoadGameState();
	_game->GameState(gameState);
}

//	Window event handlers ----------------------------------------------------------------------------------------------

void Turbo::Platform::Windows10::Windows10FrameworkView::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	_renderer->UpdateDisplayInformation();
}

void Turbo::Platform::Windows10::Windows10FrameworkView::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	_windowVisible = args->Visible;
}

void Turbo::Platform::Windows10::Windows10FrameworkView::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	_windowClosed = true;
}

//	DisplayInformation event handlers ----------------------------------------------------------------------------------

void Turbo::Platform::Windows10::Windows10FrameworkView::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	_renderer->UpdateDisplayInformation();
}

void Turbo::Platform::Windows10::Windows10FrameworkView::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	_renderer->UpdateDisplayInformation();
}

void Turbo::Platform::Windows10::Windows10FrameworkView::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	_renderer->UpdateDisplayInformation();
}

