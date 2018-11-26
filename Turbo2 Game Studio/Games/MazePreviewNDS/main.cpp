//  ========================================================================  //
//  SpaceLabyrinthWin32.cpp
//  ========================================================================  //

#include "ApplicationNDS.h"
#include "SpaceLabyrinthNDS.h"
#include "SpaceLabyrinth.h"

int main()
{
	new ApplicationNDS();
	new SpaceLabyrinthNDS();
	
	IProgram *program = new SpaceLabyrinth(factory);

	int result = application->Run(program);

	delete program;
	delete factory;
	delete application;

	return result;
}

