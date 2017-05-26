
#pragma once

#include <pch.h>

#include <ITurboGameController.h>
#include <Windows10NavigationControl.h>

using namespace Turbo::Game;

namespace Turbo
{
	namespace Platform
	{
		namespace Windows10
		{
			//  The reason that DX12NavigationControl is separate from DX12GameController 
			//	is that DX12GameController inherits from an interface in order to be abstracted, 
			//	and DX12NavigationControl has to be a "ref class" in order to receive windows events. 
			//	Apparently one class can't do both. 
			class Windows10GameController : public ITurboGameController
			{
			public:
				//  Constructors and Destructors -------------------------------------------------------------------------------
				Windows10GameController();

				//  ITurboGameController Methods -------------------------------------------------------------------------------
				virtual NavigationInfo GetNavigationInfo();

			private:
				Windows10NavigationControl^ _navigationControl;
			};
		}
	}
}