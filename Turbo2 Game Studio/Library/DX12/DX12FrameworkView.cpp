#include <pch.h>

#include <ITurboGameLevel.h>

#include <DX12Game.h>
#include <DX12FrameworkView.h>
#include <DX12Platform.h>

#include <ppltasks.h>

using namespace Application_DX12;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

// The DirectX 12 Application template is documented at http://go.microsoft.com/fwlink/?LinkID=613670&clcid=0x409

DX12FrameworkView::DX12FrameworkView() :
	_windowClosed(false),
	_windowVisible(true)
{
}

DX12FrameworkView::DX12FrameworkView(std::shared_ptr<ITurboGameLevel> level) :
	_windowClosed(false),
	_windowVisible(true),
	_level(level)
{
}

// The first method called when the IFrameworkView is being created.
void DX12FrameworkView::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &DX12FrameworkView::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &DX12FrameworkView::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &DX12FrameworkView::OnResuming);
}

// Called when the CoreWindow object is created (or re-created).
void DX12FrameworkView::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DX12FrameworkView::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DX12FrameworkView::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &DX12FrameworkView::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DX12FrameworkView::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DX12FrameworkView::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DX12FrameworkView::OnDisplayContentsInvalidated);
}

// Initializes scene resources, or loads a previously saved app state.
void DX12FrameworkView::Load(Platform::String^ entryPoint)
{
	_platform = std::shared_ptr<DX12Platform>(new DX12Platform(_level));
	_platform->Initialize();	//	Create level, create & draw static scene
}

// This method is called after the window becomes active.
void DX12FrameworkView::Run()
{
	if (_platform == nullptr)
	{
		return;
	}

	while (!_windowClosed)
	{
		if (_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			_platform->Update();
			_platform->Render();
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
void DX12FrameworkView::Uninitialize()
{
}

// Application lifecycle event handlers.

void DX12FrameworkView::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

// Notifies the app that it is being suspended.
void DX12FrameworkView::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		// TODO: Replace this with your app's suspending logic.

		// Process lifetime management may terminate suspended apps at any time, so it is
		// good practice to save any state that will allow the app to restart where it left off.

		if (_platform != nullptr) 
		{
			_platform->SaveState();
			// _platform->Suspend()    ???
		}

		// If your application uses video memory allocations that are easy to re-create,
		// consider releasing that memory to make it available to other applications.

		deferral->Complete();
	});
}

// Notifes the app that it is no longer suspended.
void DX12FrameworkView::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// TODO: Insert your code here.
	// TODO: Replace this with your app's resuming logic.
	
	if (_platform == nullptr)
	{
		return;
	}

	_platform->LoadState();
	// _platform->Resume()    ???
}

// Window event handlers.

void DX12FrameworkView::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	if (_platform == nullptr)
	{
		return;
	}

	_platform->Resize(sender->Bounds.Width, sender->Bounds.Height); //  platform has access to DeviceResources so we don't need to send width & height.
}

void DX12FrameworkView::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	_windowVisible = args->Visible;
}

void DX12FrameworkView::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	_windowClosed = true;
}

// DisplayInformation event handlers.

void DX12FrameworkView::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	
	if (_platform == nullptr)
	{
		return;
	}

	_platform->SetDPI(sender->LogicalDpi);
}

void DX12FrameworkView::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	if (_platform == nullptr)
	{
		return;
	}

	_platform->SetDisplayOrientation(sender->CurrentOrientation);
}

void DX12FrameworkView::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	if (_platform == nullptr)
	{
		return;
	}

	_platform->ValidateDevice();
}

//std::shared_ptr<DX::DeviceResources> DX12FrameworkView::GetDeviceResources()
//{
//	if (_deviceResources != nullptr && _deviceResources->IsDeviceRemoved())
//	{
//		// All references to the existing D3D device must be released before a new device
//		// can be created.
//
//		_deviceResources = nullptr;
//		_platform->DeviceResources(_deviceResources);
//	}
//
//	if (_deviceResources == nullptr)
//	{
//		_deviceResources = std::make_shared<DX::DeviceResources>();
//		_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());
//		_platform->DeviceResources(_deviceResources);
//	}
//
//	return _deviceResources;
//}

