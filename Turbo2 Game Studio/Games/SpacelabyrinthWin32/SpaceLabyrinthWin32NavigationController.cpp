
#include "pch.h"

#include "SpaceLabyrinthWin32NavigationController.h"

#pragma region Constructors and Destructors

SpaceLabyrinthWin32NavigationController::SpaceLabyrinthWin32NavigationController(IApplication *application)
{
	_applicationWin32 = dynamic_cast<IApplicationWin32*>(application);
}

SpaceLabyrinthWin32NavigationController::~SpaceLabyrinthWin32NavigationController()
{
}

#pragma endregion
#pragma region INavigationController Methods

int SpaceLabyrinthWin32NavigationController::GetNavigationInfo(NavInfo *navInfo)
{
	if (navInfo != NULL)
	{
		int x, y, status;

		_applicationWin32->GetPointer(&x, &y, &status);

		navInfo->Pointer = status & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON);
		navInfo->PointerX = x;
		navInfo->PointerY = y;

		/*
		navInfo->MoveLeft	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_LEFT);
		navInfo->MoveRight	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_RIGHT);
		navInfo->MoveDown	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_DOWN);
		navInfo->MoveUp		=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_UP);
		navInfo->MoveFore	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_INSERT);
		navInfo->MoveBack	=  _application->GetKey(VK_CONTROL) && _application->GetKey(VK_DELETE);

		navInfo->PitchFore	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_UP);
		navInfo->PitchBack	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_DOWN);
		navInfo->YawRight	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_RIGHT);
		navInfo->YawLeft	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_LEFT);
		navInfo->RollLeft	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_NUMPAD7);
		navInfo->RollRight	= !_application->GetKey(VK_CONTROL) && _application->GetKey(VK_NUMPAD9);
		*/

		navInfo->MoveLeft = 0;
		navInfo->MoveRight = 0;
		navInfo->MoveDown = 0;
		navInfo->MoveUp = 0;
		navInfo->MoveFore = _applicationWin32->GetKey(VK_INSERT) || _applicationWin32->GetKey(VK_SPACE);
		navInfo->MoveBack = _applicationWin32->GetKey(VK_DELETE) || _applicationWin32->GetKey('X');

		navInfo->PitchFore = _applicationWin32->GetKey(VK_UP) || _applicationWin32->GetKey('W');
		navInfo->PitchBack = _applicationWin32->GetKey(VK_DOWN) || _applicationWin32->GetKey('S');
		navInfo->YawRight = _applicationWin32->GetKey(VK_RIGHT) || _applicationWin32->GetKey('D');
		navInfo->YawLeft = _applicationWin32->GetKey(VK_LEFT) || _applicationWin32->GetKey('A');
		navInfo->RollLeft = 0;
		navInfo->RollRight = 0;

		navInfo->Restart = _applicationWin32->GetKey(VK_F5); _applicationWin32->SetKey(VK_F5, FALSE);

		if (navInfo->MoveFore)
			navInfo->MoveFore = navInfo->MoveFore;

		return TRUE;
	}
	return FALSE;
}

#pragma endregion
