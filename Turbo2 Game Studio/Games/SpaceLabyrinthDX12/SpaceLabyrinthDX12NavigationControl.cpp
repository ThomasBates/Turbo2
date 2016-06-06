
#include "pch.h"

#include "IApplication.h"
#include "IApplicationDX12.h"
#include "SpaceLabyrinthDX12NavigationControl.h"

using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Input;
using namespace Windows::UI;
using namespace Windows::System;
using namespace Windows::Graphics::Display;
using namespace Microsoft::WRL;
using namespace DirectX;


using namespace Application_DX12;
using namespace SpaceLabyrinthDX12;

#pragma region Constructors and Destructors

SpaceLabyrinthDX12NavigationControl::SpaceLabyrinthDX12NavigationControl()
{
	Windows::UI::Core::CoreWindow ^window = CoreWindow::GetForCurrentThread();

//	SpaceLabyrinthDX12NavigationControl::OnPointerPressed(window, nullptr);

	if (window != nullptr)
	{
		window->PointerPressed +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &SpaceLabyrinthDX12NavigationControl::OnPointerPressed);

		window->PointerMoved +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &SpaceLabyrinthDX12NavigationControl::OnPointerMoved);

		window->PointerReleased +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &SpaceLabyrinthDX12NavigationControl::OnPointerReleased);

		window->PointerExited +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &SpaceLabyrinthDX12NavigationControl::OnPointerExited);

		window->KeyDown +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &SpaceLabyrinthDX12NavigationControl::OnKeyDown);

		window->KeyUp +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &SpaceLabyrinthDX12NavigationControl::OnKeyUp);

		// There is a separate handler for mouse only relative mouse movement events.
		MouseDevice::GetForCurrentView()->MouseMoved +=
			ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &SpaceLabyrinthDX12NavigationControl::OnMouseMoved);
	}
}

//SpaceLabyrinthDX12NavigationControl::~SpaceLabyrinthDX12NavigationControl()
//{
//}

int SpaceLabyrinthDX12NavigationControl::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo == NULL)
		return FALSE;

	*navInfo = _navInfo;
	return TRUE;
}

//#pragma endregion
//#pragma region Event Handler Methods

void SpaceLabyrinthDX12NavigationControl::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
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

void SpaceLabyrinthDX12NavigationControl::OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
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

void SpaceLabyrinthDX12NavigationControl::OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
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

void SpaceLabyrinthDX12NavigationControl::OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
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

void SpaceLabyrinthDX12NavigationControl::OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
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

void SpaceLabyrinthDX12NavigationControl::OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
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

void SpaceLabyrinthDX12NavigationControl::OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args)
{
	//args->MouseDelta.
}

#pragma endregion

void SpaceLabyrinthDX12NavigationControl::ResetState()
{
	// Reset the state of the controller.
	// Disable any active pointer IDs to stop all interaction.
	m_buttonPressed = false;
	m_pausePressed = false;
	m_buttonInUse = false;
	m_moveInUse = false;
	m_lookInUse = false;
	m_fireInUse = false;
	m_mouseInUse = false;
	m_mouseLeftInUse = false;
	m_mouseRightInUse = false;
	m_movePointerID = 0;
	m_lookPointerID = 0;
	m_firePointerID = 0;
	m_mousePointerID = 0;
	m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_moveCommand = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_forward = false;
	m_back = false;
	m_left = false;
	m_right = false;
	m_up = false;
	m_down = false;
	m_pause = false;
}

