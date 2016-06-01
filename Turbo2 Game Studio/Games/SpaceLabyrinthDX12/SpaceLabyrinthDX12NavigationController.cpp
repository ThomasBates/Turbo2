
#include "pch.h"

#include "IApplication.h"
#include "IApplicationDX12.h"
#include "SpaceLabyrinthDX12NavigationController.h"

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

SpaceLabyrinthDX12NavigationController::SpaceLabyrinthDX12NavigationController(IApplication *application)
{
	_applicationDX12 = dynamic_cast<IApplicationDX12*>(application);

	Windows::UI::Core::CoreWindow ^window = CoreWindow::GetForCurrentThread();

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>( this,  &SpaceLabyrinthDX12NavigationController::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &SpaceLabyrinthDX12NavigationController::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &SpaceLabyrinthDX12NavigationController::OnPointerReleased);

	window->PointerExited +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &SpaceLabyrinthDX12NavigationController::OnPointerExited);

	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &SpaceLabyrinthDX12NavigationController::OnKeyDown);

	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &SpaceLabyrinthDX12NavigationController::OnKeyUp);

	// There is a separate handler for mouse only relative mouse movement events.
	MouseDevice::GetForCurrentView()->MouseMoved +=
		ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &SpaceLabyrinthDX12NavigationController::OnMouseMoved);
}

SpaceLabyrinthDX12NavigationController::~SpaceLabyrinthDX12NavigationController()
{
}

//#pragma endregion
//#pragma region INavigationController Methods

int SpaceLabyrinthDX12NavigationController::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo != NULL)
	{
		//int x, y, status;

		//_applicationDX12->GetPointer(&x, &y, &status);

		navInfo->Pointer = 0; // status & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON);
		navInfo->PointerX = 0; // x;
		navInfo->PointerY = 0; // y;

							   /*
							   navInfo->MoveLeft	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_LEFT);
							   navInfo->MoveRight	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_RIGHT);
							   navInfo->MoveDown	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_DOWN);
							   navInfo->MoveUp		=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_UP);
							   navInfo->MoveFore	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_INSERT);
							   navInfo->MoveBack	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_DELETE);

							   navInfo->PitchFore	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_UP);
							   navInfo->PitchBack	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_DOWN);
							   navInfo->YawRight	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_RIGHT);
							   navInfo->YawLeft	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_LEFT);
							   navInfo->RollLeft	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_NUMPAD7);
							   navInfo->RollRight	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_NUMPAD9);
							   */

		navInfo->MoveLeft = 0;
		navInfo->MoveRight = 0;
		navInfo->MoveDown = 0;
		navInfo->MoveUp = 0;
		navInfo->MoveFore = 0; // _applicationWin32->GetKey(VK_INSERT) || _applicationWin32->GetKey(VK_SPACE);
		navInfo->MoveBack = 0; // _applicationWin32->GetKey(VK_DELETE) || _applicationWin32->GetKey('X');

		navInfo->PitchFore = 0; // _applicationWin32->GetKey(VK_UP) || _applicationWin32->GetKey('W');
		navInfo->PitchBack = 0; // _applicationWin32->GetKey(VK_DOWN) || _applicationWin32->GetKey('S');
		navInfo->YawRight = 0; // _applicationWin32->GetKey(VK_RIGHT) || _applicationWin32->GetKey('D');
		navInfo->YawLeft = 0; // _applicationWin32->GetKey(VK_LEFT) || _applicationWin32->GetKey('A');
		navInfo->RollLeft = 0;
		navInfo->RollRight = 0;

		navInfo->Restart = 0; // _applicationWin32->GetKey(VK_F5); _applicationWin32->SetKey(VK_F5, FALSE);

		if (navInfo->MoveFore)
			navInfo->MoveFore = navInfo->MoveFore;

		return TRUE;
	}
	return FALSE;
}

//#pragma endregion
//#pragma region Event Handler Methods

void SpaceLabyrinthDX12NavigationController::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	PointerPoint^ point = args->CurrentPoint;

}

void SpaceLabyrinthDX12NavigationController::OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
{

}

void SpaceLabyrinthDX12NavigationController::OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
{

}

void SpaceLabyrinthDX12NavigationController::OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
{

}

void SpaceLabyrinthDX12NavigationController::OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
{

}

void SpaceLabyrinthDX12NavigationController::OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
{

}

void SpaceLabyrinthDX12NavigationController::OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args)
{

}

#pragma endregion

