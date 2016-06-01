
#pragma once

#include "pch.h"
#include <XInput.h>

#include "IApplicationDX12.h"

#include "INavigationController.h"

//using namespace Windows::Devices::Input;
//using namespace Windows::UI::Core;

using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
//using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
//using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;


using namespace Application_DX12;

namespace SpaceLabyrinthDX12
{
	class SpaceLabyrinthDX12NavigationController : public INavigationController
	{     
	private:
		IApplicationDX12 *_applicationDX12;
		//CoreWindow^ _window;

	public:
		//  Constructors and Destructors  ------------------------------------------------------------------------------
		SpaceLabyrinthDX12NavigationController(IApplication *application);
		~SpaceLabyrinthDX12NavigationController();

		//  INavigationController Methods  -----------------------------------------------------------------------------
		virtual int		GetNavigationInfo(NavInfo *navInfo);

	//protected:
		//  Event Handler Methods  -------------------------------------------------------------------------------------
		void OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args);
		void OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
		void OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
		void OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args);
	};
}