
#include <pch.h>

#include <Windows10NavigationControl.h>
#include <TurboSceneNavigationControl_Last.h>
#include <TurboSceneNavigationControl_Set.h>

using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::System;
using namespace Windows::UI::Input;

using namespace Turbo::Game;
using namespace Turbo::Platform::Windows10;


//  Constructors and Destructors ---------------------------------------------------------------------------------------

Windows10NavigationControl::Windows10NavigationControl()
{
	Windows::UI::Core::CoreWindow ^window = CoreWindow::GetForCurrentThread();

//	DX12NavigationControl::OnPointerPressed(window, nullptr);

	if (window != nullptr)
	{
		window->PointerPressed +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Windows10NavigationControl::OnPointerPressed);

		window->PointerMoved +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Windows10NavigationControl::OnPointerMoved);

		window->PointerReleased +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Windows10NavigationControl::OnPointerReleased);

		window->PointerExited +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Windows10NavigationControl::OnPointerExited);

		window->KeyDown +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &Windows10NavigationControl::OnKeyDown);

		window->KeyUp +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &Windows10NavigationControl::OnKeyUp);

		// There is a separate handler for mouse only relative mouse movement events.
		MouseDevice::GetForCurrentView()->MouseMoved +=
			ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &Windows10NavigationControl::OnMouseMoved);
	}

	_pointerLookControl = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Last(nullptr, TurboGameControlType::Look, 0.0f, 0.0f, 0.0f, 0.0f,  0.1f));
	_reverseLookControl = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Last(nullptr, TurboGameControlType::Look, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f));
	_asdwLookControl = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Set(TurboGameControlType::Look, 0.0f, 0.0f, 0.0f, 0.0f));
	_arrowKeyLookControl = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Set(TurboGameControlType::Look, 0.0f, 0.0f, 0.0f, 0.0f));
	_asdwMoveControl = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Set(TurboGameControlType::Move, 0.0f, 0.0f, 0.0f, 0.0f));
	_arrowKeyMoveControl = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Set(TurboGameControlType::Move, 0.0f, 0.0f, 0.0f, 0.0f));

	_navInfo.Controls.push_back(_pointerLookControl);
	_navInfo.Controls.push_back(_reverseLookControl);
	_navInfo.Controls.push_back(_asdwLookControl);
	_navInfo.Controls.push_back(_arrowKeyLookControl);
	_navInfo.Controls.push_back(_asdwMoveControl);
	_navInfo.Controls.push_back(_arrowKeyMoveControl);

	_keys[VirtualKey::A] = false;
	_keys[VirtualKey::S] = false;
	_keys[VirtualKey::D] = false;
	_keys[VirtualKey::W] = false;
	_keys[VirtualKey::X] = false;
	_keys[VirtualKey::Space] = false;

	_keys[VirtualKey::Down] = false;
	_keys[VirtualKey::Up] = false;
	_keys[VirtualKey::Left] = false;
	_keys[VirtualKey::Right] = false;
	_keys[VirtualKey::Insert] = false;
	_keys[VirtualKey::Delete] = false;
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  Public Access Methods ----------------------------------------------------------------------------------------------

NavigationInfo* Windows10NavigationControl::GetNavigationInfo()
{
	//  Set TimeStamp for frame
	_timer.Tick([&]()
	{
		_navInfo.Time = _timer.GetTotalSeconds();
		_navInfo.DeltaTime = _timer.GetElapsedSeconds();
	});

	return &_navInfo;
}

//  Public Access Methods ----------------------------------------------------------------------------------------------
//  Event Handler Methods ----------------------------------------------------------------------------------------------

void Windows10NavigationControl::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	PointerPoint^ point = args->CurrentPoint;
	Point pointerPosition = point->Position;
	PointerPointProperties^ pointProperties = point->Properties;

	if (pointProperties->IsLeftButtonPressed && !_pointerLookControl->IsActive())
	{
		_pointerLookControl->IsActive(true);
		_reverseLookControl->IsActive(false);

		_pointerLookControl->CurrentPoint(pointerPosition.X, pointerPosition.Y);
	}

	if (pointProperties->IsRightButtonPressed && !_reverseLookControl->IsActive())
	{
		_reverseLookControl->IsActive(true);
		_pointerLookControl->IsActive(false);

		_reverseLookControl->CurrentPoint(pointerPosition.X, pointerPosition.Y);
	}
}

void Windows10NavigationControl::OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
{
	PointerPoint^ point = args->CurrentPoint;
	Point pointerPosition = point->Position;

	if (_pointerLookControl->IsActive())
	{
		_pointerLookControl->CurrentPoint(pointerPosition.X, pointerPosition.Y);
	}

	if (_reverseLookControl->IsActive())
	{
		_reverseLookControl->CurrentPoint(pointerPosition.X, pointerPosition.Y);
	}
}

void Windows10NavigationControl::OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
{
	PointerPoint^ point = args->CurrentPoint;
	Point pointerPosition = point->Position;
	PointerPointProperties^ pointProperties = point->Properties;

	if (!pointProperties->IsLeftButtonPressed && _pointerLookControl->IsActive())
	{
		_pointerLookControl->IsActive(false);
	}

	if (!pointProperties->IsRightButtonPressed && _reverseLookControl->IsActive())
	{
		_reverseLookControl->IsActive(false);
	}
}

void Windows10NavigationControl::OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
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

void Windows10NavigationControl::OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
{
	VirtualKey key = args->VirtualKey;

	_keys[key] = true;

	UpdateKeypadControls();

	if (key == VirtualKey::P)
		_navInfo.Pause = true;

	if ((key == VirtualKey::F5) && !_lastRestart) //  Just get the rising edge of Restart. Don't keep restarting if user keeps their finger on the button.
	{
		_navInfo.Restart = true;
	}
	_lastRestart = _navInfo.Restart;
}

void Windows10NavigationControl::OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
{
	VirtualKey key = args->VirtualKey;

	_keys[key] = false;

	UpdateKeypadControls();

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

void Windows10NavigationControl::OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args)
{
	//args->MouseDelta.
}

//  Event Handler Methods ----------------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

void Windows10NavigationControl::UpdateKeypadControls()
{
	_asdwLookControl->IsActive(
		(_keys[VirtualKey::S]) ||
		(_keys[VirtualKey::W]) ||
		(_keys[VirtualKey::A]) ||
		(_keys[VirtualKey::D]));

	if (_asdwLookControl->IsActive())
	{
		double x = 0.0f;
		double y = 0.0f;

		if (_keys[VirtualKey::S])	y--;
		if (_keys[VirtualKey::W])	y++;
		if (_keys[VirtualKey::A])	x--;
		if (_keys[VirtualKey::D])	x++;

		_asdwLookControl->CurrentPoint(x, y);
	}

	_arrowKeyLookControl->IsActive(
		(_keys[VirtualKey::Down]) ||
		(_keys[VirtualKey::Up]) ||
		(_keys[VirtualKey::Left]) ||
		(_keys[VirtualKey::Right]));

	if (_arrowKeyLookControl->IsActive())
	{
		double x = 0.0f;
		double y = 0.0f;

		if (_keys[VirtualKey::Down])	y--;
		if (_keys[VirtualKey::Up])		y++;
		if (_keys[VirtualKey::Left])	x--;
		if (_keys[VirtualKey::Right])	x++;

		_arrowKeyLookControl->CurrentPoint(x, y);
	}

	_asdwMoveControl->IsActive(
		(_keys[VirtualKey::Space]) ||
		(_keys[VirtualKey::X]));

	if (_asdwMoveControl->IsActive())
	{
		double x = 0.0f;
		double y = 0.0f;

		if (_keys[VirtualKey::Space])	y--;
		if (_keys[VirtualKey::X])		y++;

		_asdwMoveControl->CurrentPoint(x, y);
	}

	_arrowKeyMoveControl->IsActive(
		(_keys[VirtualKey::Insert]) ||
		(_keys[VirtualKey::Delete]));

	if (_arrowKeyMoveControl->IsActive())
	{
		double x = 0.0f;
		double y = 0.0f;

		if (_keys[VirtualKey::Insert])	y--;
		if (_keys[VirtualKey::Delete])	y++;

		_arrowKeyMoveControl->CurrentPoint(x, y);
	}
}

//  Local Support Methods ----------------------------------------------------------------------------------------------

