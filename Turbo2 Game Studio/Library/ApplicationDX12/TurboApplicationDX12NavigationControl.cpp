
#include <pch.h>

#include <TurboApplicationDX12NavigationControl.h>

using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::System;
using namespace Windows::UI::Input;

using namespace Application_DX12;

//  Constructors and Destructors  --------------------------------------------------------------------------------------

TurboApplicationDX12NavigationControl::TurboApplicationDX12NavigationControl()
{
	Windows::UI::Core::CoreWindow ^window = CoreWindow::GetForCurrentThread();

//	TurboApplicationDX12NavigationControl::OnPointerPressed(window, nullptr);

	if (window != nullptr)
	{
		window->PointerPressed +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &TurboApplicationDX12NavigationControl::OnPointerPressed);

		window->PointerMoved +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &TurboApplicationDX12NavigationControl::OnPointerMoved);

		window->PointerReleased +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &TurboApplicationDX12NavigationControl::OnPointerReleased);

		window->PointerExited +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &TurboApplicationDX12NavigationControl::OnPointerExited);

		window->KeyDown +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &TurboApplicationDX12NavigationControl::OnKeyDown);

		window->KeyUp +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &TurboApplicationDX12NavigationControl::OnKeyUp);

		// There is a separate handler for mouse only relative mouse movement events.
		MouseDevice::GetForCurrentView()->MouseMoved +=
			ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &TurboApplicationDX12NavigationControl::OnMouseMoved);
	}
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  Public Access Methods  ---------------------------------------------------------------------------------------------

NavigationInfo TurboApplicationDX12NavigationControl::GetNavigationInfo()
{
	//  Set TimeStamp for frame
	_timer.Tick([&]()
	{
		_navInfo.Time = _timer.GetTotalSeconds();
		_navInfo.DeltaTime = _timer.GetElapsedSeconds();
	});

	return _navInfo;
}

//  Public Access Methods  ---------------------------------------------------------------------------------------------
//  Event Handler Methods  ---------------------------------------------------------------------------------------------

void TurboApplicationDX12NavigationControl::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	//PointerPoint^ point = args->CurrentPoint;
	//Point pointerPosition = point->Position;
	//PointerPointProperties^ pointProperties = point->Properties;

	//_navInfo.Pointer = 
	//	pointProperties->IsLeftButtonPressed ||
	//	pointProperties->IsMiddleButtonPressed ||
	//	pointProperties->IsRightButtonPressed;

	//_navInfo.PointerX = pointerPosition.X;
	//_navInfo.PointerY = pointerPosition.Y;
}

void TurboApplicationDX12NavigationControl::OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
{
	PointerPoint^ point = args->CurrentPoint;
	Point pointerPosition = point->Position;
	PointerPointProperties^ pointProperties = point->Properties;

	_navInfo.Pointer =
		pointProperties->IsLeftButtonPressed ||
		pointProperties->IsMiddleButtonPressed ||
		pointProperties->IsRightButtonPressed;

	_navInfo.PointerX = pointerPosition.X;
	_navInfo.PointerY = pointerPosition.Y;
}

void TurboApplicationDX12NavigationControl::OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
{
	//PointerPoint^ point = args->CurrentPoint;
	//Point pointerPosition = point->Position;
	//PointerPointProperties^ pointProperties = point->Properties;

	//_navInfo.Pointer =
	//	pointProperties->IsLeftButtonPressed ||
	//	pointProperties->IsMiddleButtonPressed ||
	//	pointProperties->IsRightButtonPressed;

	//_navInfo.PointerX = pointerPosition.X;
	//_navInfo.PointerY = pointerPosition.Y;
}

void TurboApplicationDX12NavigationControl::OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
{
	//PointerPoint^ point = args->CurrentPoint;
	//Point pointerPosition = point->Position;
	//PointerPointProperties^ pointProperties = point->Properties;

	//_navInfo.Pointer =
	//	pointProperties->IsLeftButtonPressed ||
	//	pointProperties->IsMiddleButtonPressed ||
	//	pointProperties->IsRightButtonPressed;

	//_navInfo.PointerX = pointerPosition.X;
	//_navInfo.PointerY = pointerPosition.Y;
}

void TurboApplicationDX12NavigationControl::OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if ((Key == VirtualKey::Up) || (Key == VirtualKey::W))
		_navInfo.PitchFore = true;
	if ((Key == VirtualKey::Down) || (Key == VirtualKey::S))
		_navInfo.PitchBack = true;
	if ((Key == VirtualKey::Left) || (Key == VirtualKey::A))
		_navInfo.YawLeft = true;
	if ((Key == VirtualKey::Right) || (Key == VirtualKey::D))
		_navInfo.YawRight = true;
	if ((Key == VirtualKey::Insert) || (Key == VirtualKey::Space))
		_navInfo.MoveFore = true;
	if ((Key == VirtualKey::Delete) || (Key == VirtualKey::X))
		_navInfo.MoveBack = true;
	if (Key == VirtualKey::P)
		_navInfo.Pause = true;
	if (Key == VirtualKey::F5)
		_navInfo.Restart = true;
}

void TurboApplicationDX12NavigationControl::OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if ((Key == VirtualKey::Up) || (Key == VirtualKey::W))
		_navInfo.PitchFore = false;
	if ((Key == VirtualKey::Down) || (Key == VirtualKey::S))
		_navInfo.PitchBack = false;
	if ((Key == VirtualKey::Left) || (Key == VirtualKey::A))
		_navInfo.YawLeft = false;
	if ((Key == VirtualKey::Right) || (Key == VirtualKey::D))
		_navInfo.YawRight = false;
	if ((Key == VirtualKey::Insert) || (Key == VirtualKey::Space))
		_navInfo.MoveFore = false;
	if ((Key == VirtualKey::Delete) || (Key == VirtualKey::X))
		_navInfo.MoveBack = false;
	if (Key == VirtualKey::P)
		_navInfo.Pause = false;
	if (Key == VirtualKey::F5)
		_navInfo.Restart = false;
	//if (Key == VirtualKey::P)
	//{
	//	if (m_pause)
	//	{
	//		// Trigger pause only once on button release.
	//		m_pausePressed = true;
	//		m_pause = false;
	//	}
	//}
}

void TurboApplicationDX12NavigationControl::OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args)
{
	//args->MouseDelta.
}

//  Event Handler Methods  ---------------------------------------------------------------------------------------------
//  Local Support Methods  ---------------------------------------------------------------------------------------------

//  Local Support Methods  ---------------------------------------------------------------------------------------------

