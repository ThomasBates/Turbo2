
#include <pch.h>

#include <Windows10NavigationControl.h>

using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::System;
using namespace Windows::UI::Input;

using namespace Turbo::Game;


//  Constructors and Destructors ---------------------------------------------------------------------------------------

Turbo::Platform::Windows10::Windows10NavigationControl::Windows10NavigationControl()
{
	Windows::UI::Core::CoreWindow ^window = CoreWindow::GetForCurrentThread();

//	DX12NavigationControl::OnPointerPressed(window, nullptr);

	if (window != nullptr)
	{
		window->PointerPressed +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Turbo::Platform::Windows10::Windows10NavigationControl::OnPointerPressed);

		window->PointerMoved +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Turbo::Platform::Windows10::Windows10NavigationControl::OnPointerMoved);

		window->PointerReleased +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Turbo::Platform::Windows10::Windows10NavigationControl::OnPointerReleased);

		window->PointerExited +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Turbo::Platform::Windows10::Windows10NavigationControl::OnPointerExited);

		window->KeyDown +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &Turbo::Platform::Windows10::Windows10NavigationControl::OnKeyDown);

		window->KeyUp +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &Turbo::Platform::Windows10::Windows10NavigationControl::OnKeyUp);

		// There is a separate handler for mouse only relative mouse movement events.
		MouseDevice::GetForCurrentView()->MouseMoved +=
			ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &Turbo::Platform::Windows10::Windows10NavigationControl::OnMouseMoved);
	}
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  Public Access Methods ----------------------------------------------------------------------------------------------

NavigationInfo Turbo::Platform::Windows10::Windows10NavigationControl::GetNavigationInfo()
{
	//  Set TimeStamp for frame
	_timer.Tick([&]()
	{
		_navInfo.Time = _timer.GetTotalSeconds();
		_navInfo.DeltaTime = _timer.GetElapsedSeconds();
	});

	return _navInfo;
}

//  Public Access Methods ----------------------------------------------------------------------------------------------
//  Event Handler Methods ----------------------------------------------------------------------------------------------

void Turbo::Platform::Windows10::Windows10NavigationControl::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
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

void Turbo::Platform::Windows10::Windows10NavigationControl::OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
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

void Turbo::Platform::Windows10::Windows10NavigationControl::OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
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

void Turbo::Platform::Windows10::Windows10NavigationControl::OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
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

void Turbo::Platform::Windows10::Windows10NavigationControl::OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
{
	Windows::System::VirtualKey key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if ((key == VirtualKey::Down) || (key == VirtualKey::S))
		_navInfo.PitchFore = true;
	if ((key == VirtualKey::Up) || (key == VirtualKey::W))
		_navInfo.PitchBack = true;
	if ((key == VirtualKey::Left) || (key == VirtualKey::A))
		_navInfo.YawLeft = true;
	if ((key == VirtualKey::Right) || (key == VirtualKey::D))
		_navInfo.YawRight = true;
	if ((key == VirtualKey::Insert) || (key == VirtualKey::Space))
		_navInfo.MoveFore = true;
	if ((key == VirtualKey::Delete) || (key == VirtualKey::X))
		_navInfo.MoveBack = true;
	if (key == VirtualKey::P)
		_navInfo.Pause = true;
	if ((key == VirtualKey::F5) && !_lastRestart) //  Just get the rising edge of Restart. Don't keep restarting if user keeps their finger on the button.
	{
		_navInfo.Restart = true;
	}
	_lastRestart = _navInfo.Restart;
}

void Turbo::Platform::Windows10::Windows10NavigationControl::OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
{
	Windows::System::VirtualKey key;
	key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if ((key == VirtualKey::Down) || (key == VirtualKey::S))
		_navInfo.PitchFore = false;
	if ((key == VirtualKey::Up) || (key == VirtualKey::W))
		_navInfo.PitchBack = false;
	if ((key == VirtualKey::Left) || (key == VirtualKey::A))
		_navInfo.YawLeft = false;
	if ((key == VirtualKey::Right) || (key == VirtualKey::D))
		_navInfo.YawRight = false;
	if ((key == VirtualKey::Insert) || (key == VirtualKey::Space))
		_navInfo.MoveFore = false;
	if ((key == VirtualKey::Delete) || (key == VirtualKey::X))
		_navInfo.MoveBack = false;
	if (key == VirtualKey::P)
		_navInfo.Pause = false;
	if (key == VirtualKey::F5)
		_navInfo.Restart = false;
	//if (key == VirtualKey::P)
	//{
	//	if (m_pause)
	//	{
	//		// Trigger pause only once on button release.
	//		m_pausePressed = true;
	//		m_pause = false;
	//	}
	//}
}

void Turbo::Platform::Windows10::Windows10NavigationControl::OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args)
{
	//args->MouseDelta.
}

//  Event Handler Methods ----------------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

//  Local Support Methods ----------------------------------------------------------------------------------------------
