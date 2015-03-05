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
	new ApplicationWin32(TEXT("Space Labyrinth"));
	new SpaceLabyrinthWin32();
	new SpaceLabyrinthProgram();

	return Application->Run();
}

