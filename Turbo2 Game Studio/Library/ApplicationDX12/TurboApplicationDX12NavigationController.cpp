
#include <pch.h>

#include <TurboApplicationDX12NavigationController.h>

using namespace Application_DX12;

//  Constructors and Destructors  --------------------------------------------------------------------------------------

TurboApplicationDX12NavigationController::TurboApplicationDX12NavigationController()
{
	_navigationControl = ref new TurboApplicationDX12NavigationControl();
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  INavigationController Methods  -------------------------------------------------------------------------------------

NavigationInfo TurboApplicationDX12NavigationController::GetNavigationInfo()
{
	return _navigationControl->GetNavigationInfo();
}

//  INavigationController Methods  -------------------------------------------------------------------------------------
