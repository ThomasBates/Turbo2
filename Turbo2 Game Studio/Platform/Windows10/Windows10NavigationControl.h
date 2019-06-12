
#pragma once

#include <pch.h>

#include <Windows10Timer.h>
#include <ITurboGameController.h>
#include <ITurboDebug.h>

using namespace Windows::Devices::Input;
using namespace Windows::System;
using namespace Windows::UI::Core;

using namespace Turbo::Core::Debug;

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
			ref class Windows10NavigationControl
			{
			internal:
				//  Constructors and Destructors -------------------------------------------------------------------------------
				Windows10NavigationControl();

				//  Public Access Methods --------------------------------------------------------------------------------------
				NavigationInfo* GetNavigationInfo();

			private:
				NavigationInfo _navInfo;
				Timer _timer;
				bool _lastRestart;

				std::map<VirtualKey, bool> _keys;

				std::shared_ptr<ITurboSceneNavigationControl> _pointerLookControl;
				std::shared_ptr<ITurboSceneNavigationControl> _reverseLookControl;
				std::shared_ptr<ITurboSceneNavigationControl> _asdwLookControl;
				std::shared_ptr<ITurboSceneNavigationControl> _arrowKeyLookControl;
				std::shared_ptr<ITurboSceneNavigationControl> _asdwMoveControl;
				std::shared_ptr<ITurboSceneNavigationControl> _arrowKeyMoveControl;

				//  Event Handler Methods --------------------------------------------------------------------------------------
				void OnPointerPressed(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
				void OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
				void OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
				void OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
				void OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
				void OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
				void OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args);

				//  Local Support Methods --------------------------------------------------------------------------------------
				void UpdateKeypadControls();
			};
		}
	}
}