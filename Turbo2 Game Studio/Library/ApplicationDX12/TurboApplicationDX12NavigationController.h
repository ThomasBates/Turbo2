
#pragma once

#include <pch.h>

#include <INavigationController.h>
#include <TurboApplicationDX12NavigationControl.h>

namespace Application_DX12
{
	//  The reason that TurboApplicationDX12NavigationControl is separate from TurboApplicationDX12NavigationController 
	//	is that TurboApplicationDX12NavigationController inherits from an interface in order to be abstracted, 
	//	and TurboApplicationDX12NavigationControl has to be a "ref class" in order to receive windows events. 
	//	Apparently one class can't do both. 
	class TurboApplicationDX12NavigationController : public INavigationController
	{     
	public:
		//  Constructors and Destructors  ------------------------------------------------------------------------------
		TurboApplicationDX12NavigationController();

		//  INavigationController Methods  -----------------------------------------------------------------------------
		virtual NavigationInfo GetNavigationInfo();

	private:
		TurboApplicationDX12NavigationControl^ _navigationControl;
	};
}