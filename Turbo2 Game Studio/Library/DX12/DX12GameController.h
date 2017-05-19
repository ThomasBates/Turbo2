
#pragma once

#include <pch.h>

#include <ITurboGameController.h>
#include <DX12NavigationControl.h>

namespace Application_DX12
{
	//  The reason that DX12NavigationControl is separate from DX12GameController 
	//	is that DX12GameController inherits from an interface in order to be abstracted, 
	//	and DX12NavigationControl has to be a "ref class" in order to receive windows events. 
	//	Apparently one class can't do both. 
	class DX12GameController : public ITurboGameController
	{     
	public:
		//  Constructors and Destructors -------------------------------------------------------------------------------
		DX12GameController();

		//  ITurboGameController Methods -------------------------------------------------------------------------------
		virtual NavigationInfo GetNavigationInfo();

	private:
		DX12NavigationControl^ _navigationControl;
	};
}