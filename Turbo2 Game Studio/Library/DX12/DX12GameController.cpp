
#include <pch.h>

#include <DX12GameController.h>

using namespace Application_DX12;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

DX12GameController::DX12GameController()
{
	_navigationControl = ref new DX12NavigationControl();
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameController Methods ---------------------------------------------------------------------------------------

NavigationInfo DX12GameController::GetNavigationInfo()
{
	return _navigationControl->GetNavigationInfo();
}

//  ITurboGameController Methods ---------------------------------------------------------------------------------------
