
#pragma once

#include <pch.h>

#include <Common\StepTimer.h>
#include <ITurboGameController.h>

using namespace Windows::Devices::Input;
using namespace Windows::UI::Core;

namespace Application_DX12
{
	//  The reason that DX12NavigationControl is separate from DX12GameController 
	//	is that DX12GameController inherits from an interface in order to be abstracted, 
	//	and DX12NavigationControl has to be a "ref class" in order to receive windows events. 
	//	Apparently one class can't do both. 
	ref class DX12NavigationControl 
	{
	internal:
		//  Constructors and Destructors -------------------------------------------------------------------------------
		DX12NavigationControl();

		//  Public Access Methods --------------------------------------------------------------------------------------
		NavigationInfo GetNavigationInfo();

	private:
		NavigationInfo _navInfo;
		DX::StepTimer _timer;
		bool _lastRestart;

		//  Event Handler Methods --------------------------------------------------------------------------------------
		void OnPointerPressed(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
		void OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
		void OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args);

		//  Local Support Methods --------------------------------------------------------------------------------------
	};
}