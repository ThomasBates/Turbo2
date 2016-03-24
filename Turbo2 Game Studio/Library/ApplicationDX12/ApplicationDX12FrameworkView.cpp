#include "pch.h"

#include "IProgram.h"

#include "ApplicationDX12.h"
#include "ApplicationDX12PlatformResources.h"
#include "ApplicationDX12FrameworkView.h"

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

ApplicationDX12FrameworkView::ApplicationDX12FrameworkView() :
	m_windowClosed(false),
	m_windowVisible(true)
{
}

ApplicationDX12FrameworkView::ApplicationDX12FrameworkView(IProgram *program) :
	m_windowClosed(false),
	m_windowVisible(true),
	_program(program)
{
}

// The first method called when the IFrameworkView is being created.
void ApplicationDX12FrameworkView::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &ApplicationDX12FrameworkView::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &ApplicationDX12FrameworkView::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &ApplicationDX12FrameworkView::OnResuming);
}

// Called when the CoreWindow object is created (or re-created).
void ApplicationDX12FrameworkView::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &ApplicationDX12FrameworkView::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &ApplicationDX12FrameworkView::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &ApplicationDX12FrameworkView::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &ApplicationDX12FrameworkView::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &ApplicationDX12FrameworkView::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &ApplicationDX12FrameworkView::OnDisplayContentsInvalidated);
}

// Initializes scene resources, or loads a previously saved app state.
void ApplicationDX12FrameworkView::Load(Platform::String^ entryPoint)
{
}

// This method is called after the window becomes active.
void ApplicationDX12FrameworkView::Run()
{
	if (_program == nullptr)
		return;

	if (!_program->Initialize())
		return;

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			auto commandQueue = GetDeviceResources()->GetCommandQueue();
			PIXBeginEvent(commandQueue, 0, L"Update");
			{
				_program->Update();
			}
			PIXEndEvent(commandQueue);

			PIXBeginEvent(commandQueue, 0, L"Render");
			{
				if (_program->Render())
				{
					GetDeviceResources()->Present();
				}
			}
			PIXEndEvent(commandQueue);
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
void ApplicationDX12FrameworkView::Uninitialize()
{
}

// Application lifecycle event handlers.

void ApplicationDX12FrameworkView::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

// Notifies the app that it is being suspended.
void ApplicationDX12FrameworkView::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
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

		_program->SaveState();
		// _program->Suspend()    ???

		// If your application uses video memory allocations that are easy to re-create,
		// consider releasing that memory to make it available to other applications.

		deferral->Complete();
	});
}

// Notifes the app that it is no longer suspended.
void ApplicationDX12FrameworkView::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// TODO: Insert your code here.
	// TODO: Replace this with your app's resuming logic.
	
	// _program->LoadState() ???
	// _program->Resume()    ???
}

// Window event handlers.

void ApplicationDX12FrameworkView::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	GetDeviceResources()->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	_program->Resize(0, 0); //  platform has access to DeviceResources to we don't need to send width & height.
}

void ApplicationDX12FrameworkView::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void ApplicationDX12FrameworkView::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

// DisplayInformation event handlers.

void ApplicationDX12FrameworkView::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	GetDeviceResources()->SetDpi(sender->LogicalDpi);
	_program->Resize(0, 0); //  platform has access to DeviceResources to we don't need to send width & height.
}

void ApplicationDX12FrameworkView::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	GetDeviceResources()->SetCurrentOrientation(sender->CurrentOrientation);
	_program->Resize(0, 0); //  platform has access to DeviceResources to we don't need to send width & height.
}

void ApplicationDX12FrameworkView::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	GetDeviceResources()->ValidateDevice();
}

std::shared_ptr<DX::DeviceResources> ApplicationDX12FrameworkView::GetDeviceResources()
{
	if (m_deviceResources != nullptr && m_deviceResources->IsDeviceRemoved())
	{
		// All references to the existing D3D device must be released before a new device
		// can be created.

		m_deviceResources = nullptr;
		_program->SaveState();
		_program->SetPlatformResources(nullptr);
	}

	if (m_deviceResources == nullptr)
	{
		m_deviceResources = std::make_shared<DX::DeviceResources>();
		m_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());
		
		IPlatformResources *platformResources = new ApplicationDX12PlatformResources(m_deviceResources);
		_program->SetPlatformResources(platformResources);
		_program->Resize(0, 0); //  platform has access to DeviceResources to we don't need to send width & height.
	}

	return m_deviceResources;
}

