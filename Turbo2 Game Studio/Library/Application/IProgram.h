
#pragma once

#include <pch.h>

#include <IApplicationState.h>
#include <IGameLevel.h>

class IProgram
{
public:
	//  IProgram Properties  -------------------------------------------------------------------------------------------
	virtual LPCWSTR Title() = 0;
	//virtual std::shared_ptr<IGameLevel> Level() = 0;
	
	virtual std::shared_ptr<IApplicationState> State() = 0;
	virtual void State(std::shared_ptr<IApplicationState> state) = 0;

	virtual std::shared_ptr<ITurboScene> StaticScene() = 0;
	virtual std::shared_ptr<ITurboScene> DynamicScene() = 0;

	virtual bool NeedToRedrawStaticScene() = 0;

	//  IProgram Methods  ----------------------------------------------------------------------------------------------
	virtual void	Initialize() = 0;
	virtual void	Finalize() = 0;
	virtual void	Update(NavigationInfo navInfo) = 0;
};