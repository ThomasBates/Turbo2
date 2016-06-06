
#pragma once

#include "pch.h"
#include <XInput.h>

#include "IApplicationDX12.h"

#include "INavigationController.h"
#include "SpaceLabyrinthDX12NavigationControl.h"

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
	public:
		//  Constructors and Destructors  ------------------------------------------------------------------------------
		SpaceLabyrinthDX12NavigationController(IApplication *application);
		~SpaceLabyrinthDX12NavigationController();

		//  INavigationController Methods  -----------------------------------------------------------------------------
		virtual int		GetNavigationInfo(NavInfo *navInfo);

	private:
		IApplicationDX12 *_applicationDX12;
		//CoreWindow^ _window;
		SpaceLabyrinthDX12NavigationControl^ _navigationControl;
	};
}