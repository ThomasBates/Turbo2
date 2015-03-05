//  ========================================================================  //
//  SpaceLabyrinthWin32.cpp
//  ========================================================================  //

#include "ApplicationNDS.h"
#include "SpaceLabyrinthNDS.h"
#include "SpaceLabyrinth.h"

int main()
{
	ApplicationNDS *application = new ApplicationNDS();

	ISpaceLabyrinthFactory *factory = new SpaceLabyrinthNDS(application);
	IProgram *program = new SpaceLabyrinth(factory);

	int result = application->Run(program);

	delete program;
	delete factory;
	delete application;

	return result;
}

