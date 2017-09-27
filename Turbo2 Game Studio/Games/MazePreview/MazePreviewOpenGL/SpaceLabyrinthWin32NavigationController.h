
#pragma once

#include "IApplicationWin32.h"

#include "IApplication.h"
#include "INavigationController.h"

//using namespace Application_DX12;

class SpaceLabyrinthWin32NavigationController : public INavigationController
{     
private:
	IApplicationWin32 *_applicationWin32;

public:
	//  Constructors and Destructors
	SpaceLabyrinthWin32NavigationController(IApplication *application);
	~SpaceLabyrinthWin32NavigationController();

	//  INavigationController Methods
	virtual int		GetNavigationInfo(NavInfo *navInfo);
};
