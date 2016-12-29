
#include "pch.h"

#include "IApplication.h"
#include "TurboApplicationDX12NavigationController.h"

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

#pragma region Constructors and Destructors

TurboApplicationDX12NavigationController::TurboApplicationDX12NavigationController(std::shared_ptr<IApplication> application)
{
	//_navigationControl = ref new TurboApplicationDX12NavigationControl();
}

TurboApplicationDX12NavigationController::~TurboApplicationDX12NavigationController()
{
}

//#pragma endregion
//#pragma region INavigationController Methods

void TurboApplicationDX12NavigationController::SetTimeStampForFrame()
{
	if (_navigationControl == nullptr)
	{
		_navigationControl = ref new TurboApplicationDX12NavigationControl();
	}

	_navigationControl->SetTimeStampForFrame();
}

NavigationInfo TurboApplicationDX12NavigationController::GetNavigationInfo()
{
	if (_navigationControl == nullptr)
	{
		_navigationControl = ref new TurboApplicationDX12NavigationControl();
	}

	if (_navigationControl != nullptr)
	{
		return _navigationControl->GetNavigationInfo();
	}
}

//#pragma endregion
//#pragma region Event Handler Methods

//void TurboApplicationDX12NavigationController::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
//{
//	PointerPoint^ point = args->CurrentPoint;
//
//}
//
//void TurboApplicationDX12NavigationController::OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
//{
//
//}
//
//void TurboApplicationDX12NavigationController::OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
//{
//
//}
//

//void TurboApplicationDX12NavigationController::OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args)
//{
//
//}
//
//void TurboApplicationDX12NavigationController::OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
//{
//
//}
//
//void TurboApplicationDX12NavigationController::OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args)
//{
//
//}
//
//void TurboApplicationDX12NavigationController::OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args)
//{
//
//}

#pragma endregion

