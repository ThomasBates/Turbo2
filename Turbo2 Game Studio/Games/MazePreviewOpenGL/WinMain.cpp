//  ========================================================================  //
//  WinMain.cpp
//  ========================================================================  //

#include <windows.h>							// Header File For Windows

#include "ApplicationWin32.h"
#include "SpaceLabyrinthWin32Platform.h"
#include "SpaceLabyrinth.h"

int WINAPI WinMain(	HINSTANCE	hInstance,		// Instance
					HINSTANCE	hPrevInstance,	// Previous Instance
					LPSTR		lpCmdLine,		// Command Line Parameters
					int			nCmdShow)		// Window Show State
{
	IApplication *application = new ApplicationWin32(TEXT("Space Labyrinth"));
	ISpaceLabyrinthPlatform *platform = new SpaceLabyrinthWin32Platform(application);
	IProgram *program = new SpaceLabyrinth(platform);

	application->Run(program);

	return 0;
}

