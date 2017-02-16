
#pragma once

#include <pch.h>

#include <Common\StepTimer.h>
#include <INavigationController.h>

using namespace Windows::Devices::Input;
using namespace Windows::UI::Core;

namespace Application_DX12
{
	//  The reason that TurboApplicationDX12NavigationControl is separate from TurboApplicationDX12NavigationController 
	//	is that TurboApplicationDX12NavigationController inherits from an interface in order to be abstracted, 
	//	and TurboApplicationDX12NavigationControl has to be a "ref class" in order to receive windows events. 
	//	Apparently one class can't do both. 
	ref class TurboApplicationDX12NavigationControl 
	{
	internal:
		//  Constructors and Destructors  ------------------------------------------------------------------------------
		TurboApplicationDX12NavigationControl();

		//  Public Access Methods  -------------------------------------------------------------------------------------
		NavigationInfo GetNavigationInfo();

	private:
		NavigationInfo _navInfo;
		DX::StepTimer _timer;

		//  Event Handler Methods  -------------------------------------------------------------------------------------
		void OnPointerPressed(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
		void OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
		void OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args);

		//  Local Support Methods  -------------------------------------------------------------------------------------
	};
}