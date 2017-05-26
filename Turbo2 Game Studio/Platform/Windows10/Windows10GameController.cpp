
#include <pch.h>

#include <Windows10GameController.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

Turbo::Platform::Windows10::Windows10GameController::Windows10GameController()
{
	_navigationControl = ref new Windows10NavigationControl();
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameController Methods ---------------------------------------------------------------------------------------

NavigationInfo Turbo::Platform::Windows10::Windows10GameController::GetNavigationInfo()
{
	return _navigationControl->GetNavigationInfo();
}

//  ITurboGameController Methods ---------------------------------------------------------------------------------------
