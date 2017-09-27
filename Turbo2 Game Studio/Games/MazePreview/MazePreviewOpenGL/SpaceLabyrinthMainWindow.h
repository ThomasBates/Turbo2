

#pragma once

#include "Window.h"
#include "ISpaceLabyrinthPlatform.h"
//	#include "ISpaceLabyrinthMainWindow.h"

class SpaceLabyrinthMainWindow : public Window	//	, public ISpaceLabyrinthMainWindow
{
	//	Constructors
	SpaceLabyrinthMainWindow(ISpaceLabyrinthPlatform *platform);
};