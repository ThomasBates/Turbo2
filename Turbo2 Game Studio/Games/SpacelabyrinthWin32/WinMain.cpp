//  ========================================================================  //
//  SpaceLabyrinthWin32.cpp
//  ========================================================================  //

#include <windows.h>							// Header File For Windows

#include "ApplicationWin32.h"
#include "SpaceLabyrinthWin32.h"
#include "SpaceLabyrinth.h"

int WINAPI WinMain(	HINSTANCE	hInstance,		// Instance
					HINSTANCE	hPrevInstance,	// Previous Instance
					LPSTR		lpCmdLine,		// Command Line Parameters
					int			nCmdShow)		// Window Show State
{
	ApplicationWin32 *application = new ApplicationWin32(TEXT("Space Labyrinth"));

	ISpaceLabyrinthFactory *factory = new SpaceLabyrinthWin32(application);
	IProgram *program = new SpaceLabyrinth(factory);

	int result = application->Run(program);

	delete program;
	delete factory;
	delete application;

	return result;
}

