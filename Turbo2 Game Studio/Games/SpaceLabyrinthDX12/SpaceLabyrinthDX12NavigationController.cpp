
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

	//_navigationControl = ref new SpaceLabyrinthDX12NavigationControl();
}

SpaceLabyrinthDX12NavigationController::~SpaceLabyrinthDX12NavigationController()
{
}

//#pragma endregion
//#pragma region INavigationController Methods

int SpaceLabyrinthDX12NavigationController::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo != nullptr)
	{
		if (_navigationControl == nullptr)
		{
			Windows::UI::Core::CoreWindow ^window = CoreWindow::GetForCurrentThread();
			if (window != nullptr)
				_navigationControl = ref new SpaceLabyrinthDX12NavigationControl();
		}

		if (_navigationControl != nullptr)
		{
			_navigationControl->GetNavigationInfo(navInfo);
			return TRUE;
		}
	}
	return FALSE;
}

//#pragma endregion
//#pragma region Event Handler Methods

//void SpaceLabyrinthDX12NavigationController::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
//{
//	PointerPoint^ point = args->CurrentPoint;
//
//}
//
//void SpaceLabyrinthDX12NavigationController::OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
//{
//
//}
//
//void SpaceLabyrinthDX12NavigationController::OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
//{
//
//}
//
//void SpaceLabyrinthDX12NavigationController::OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
//{
//
//}
//
//void SpaceLabyrinthDX12NavigationController::OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
//{
//
//}
//
//void SpaceLabyrinthDX12NavigationController::OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
//{
//
//}
//
//void SpaceLabyrinthDX12NavigationController::OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args)
//{
//
//}

#pragma endregion

