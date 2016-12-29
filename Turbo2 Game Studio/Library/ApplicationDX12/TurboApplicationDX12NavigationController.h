
#pragma once

#include "pch.h"
#include <XInput.h>

#include "INavigationController.h"
#include "TurboApplicationDX12NavigationControl.h"

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


namespace Application_DX12
{
	class TurboApplicationDX12NavigationController : public INavigationController
	{     
	public:
		//  Constructors and Destructors  ------------------------------------------------------------------------------
		TurboApplicationDX12NavigationController(std::shared_ptr<IApplication> application);
		~TurboApplicationDX12NavigationController();

		//  INavigationController Methods  -----------------------------------------------------------------------------
		virtual void SetTimeStampForFrame();
		virtual NavigationInfo GetNavigationInfo();

	private:
		//CoreWindow^ _window;
		TurboApplicationDX12NavigationControl^ _navigationControl;
	};
}