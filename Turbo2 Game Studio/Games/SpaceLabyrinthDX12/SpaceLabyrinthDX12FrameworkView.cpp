#include "pch.h"

#include "IProgram.h"

#include "SpaceLabyrinthDX12FrameworkView.h"
#include "ApplicationDX12.h"

#include <ppltasks.h>

using namespace SpaceLabyrinthDX12;

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

SpaceLabyrinthDX12FrameworkView::SpaceLabyrinthDX12FrameworkView() :
	m_windowClosed(false),
	m_windowVisible(true)
{
	//  This is a little hack using a global "Application" because IFrameworkViewSource and IFrameworkView
	//	are both sealed so I couldn't inject IProgram through the constructor, and I couldn't figure out 
	//	another way to pass IProgram from IApplication to here.
	//_program = Application->GetProgram();
}

// The first method called when the IFrameworkView is being created.
void SpaceLabyrinthDX12FrameworkView::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &SpaceLabyrinthDX12FrameworkView::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &SpaceLabyrinthDX12FrameworkView::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &SpaceLabyrinthDX12FrameworkView::OnResuming);
}

// Called when the CoreWindow object is created (or re-created).
void SpaceLabyrinthDX12FrameworkView::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &SpaceLabyrinthDX12FrameworkView::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &SpaceLabyrinthDX12FrameworkView::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &SpaceLabyrinthDX12FrameworkView::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &SpaceLabyrinthDX12FrameworkView::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &SpaceLabyrinthDX12FrameworkView::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &SpaceLabyrinthDX12FrameworkView::OnDisplayContentsInvalidated);
}

// Initializes scene resources, or loads a previously saved app state.
void SpaceLabyrinthDX12FrameworkView::Load(Platform::String^ entryPoint)
{
	if (m_main == nullptr)
	{
		m_main = std::unique_ptr<SpaceLabyrinthDX12Main>(new SpaceLabyrinthDX12Main(_program));
	}
}

// This method is called after the window becomes active.
void SpaceLabyrinthDX12FrameworkView::Run()
{
	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			auto commandQueue = GetDeviceResources()->GetCommandQueue();
			PIXBeginEvent(commandQueue, 0, L"Update");
			{
				m_main->Update();
			}
			PIXEndEvent(commandQueue);

			PIXBeginEvent(commandQueue, 0, L"Render");
			{
				if (m_main->Render())
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
void SpaceLabyrinthDX12FrameworkView::Uninitialize()
{
}

// Application lifecycle event handlers.

void SpaceLabyrinthDX12FrameworkView::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

void SpaceLabyrinthDX12FrameworkView::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		// TODO: Insert your code here.
		m_main->OnSuspending();

		deferral->Complete();
	});
}

void SpaceLabyrinthDX12FrameworkView::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// TODO: Insert your code here.
	m_main->OnResuming();
}

// Window event handlers.

void SpaceLabyrinthDX12FrameworkView::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	GetDeviceResources()->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	m_main->OnWindowSizeChanged();
}

void SpaceLabyrinthDX12FrameworkView::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void SpaceLabyrinthDX12FrameworkView::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

// DisplayInformation event handlers.

void SpaceLabyrinthDX12FrameworkView::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	GetDeviceResources()->SetDpi(sender->LogicalDpi);
	m_main->OnWindowSizeChanged();
}

void SpaceLabyrinthDX12FrameworkView::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	GetDeviceResources()->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->OnWindowSizeChanged();
}

void SpaceLabyrinthDX12FrameworkView::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	GetDeviceResources()->ValidateDevice();
}

std::shared_ptr<DX::DeviceResources> SpaceLabyrinthDX12FrameworkView::GetDeviceResources()
{
	if (m_deviceResources != nullptr && m_deviceResources->IsDeviceRemoved())
	{
		// All references to the existing D3D device must be released before a new device
		// can be created.

		m_deviceResources = nullptr;
		m_main->OnDeviceRemoved();
	}

	if (m_deviceResources == nullptr)
	{
		m_deviceResources = std::make_shared<DX::DeviceResources>();
		m_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());
		m_main->CreateRenderers(m_deviceResources);
	}
	return m_deviceResources;
}
