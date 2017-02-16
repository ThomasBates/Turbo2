
#pragma once

#include "pch.h"

#include "IProgram.h"
#include "IGameLevel.h"

class SpaceLabyrinth: public IProgram
{
public:
	//  Constructors and Destructors  ----------------------------------------------------------------------------------
	SpaceLabyrinth();

	//  IProgram Properties  -------------------------------------------------------------------------------------------
	virtual LPCWSTR Title() { return TEXT("Space Labyrinth"); }
	//virtual std::shared_ptr<IGameLevel> Level() { return _level; }

	virtual std::shared_ptr<IApplicationState> State();
	virtual void State(std::shared_ptr<IApplicationState> state);

	virtual std::shared_ptr<ITurboScene> StaticScene();
	virtual std::shared_ptr<ITurboScene> DynamicScene();

	virtual bool NeedToRedrawStaticScene() { return _needToRedrawStaticScene; }

	//  IProgram Methods  ----------------------------------------------------------------------------------------------
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update(NavigationInfo navInfo);

private:
	std::shared_ptr<IGameLevel> _level = nullptr;
	bool _lastRestart = false;
	bool _needToRedrawStaticScene = false;

};

