
#pragma once

#include <INavigationController.h>
#include <ITurboScene.h>

class IGameLevel
{
public:
	//	IGameLevel Properties  ---------------------------------------------------------------------------------------------
	virtual std::shared_ptr<IApplicationState> State() = 0;
	virtual void State(std::shared_ptr<IApplicationState> state) = 0;

	virtual std::shared_ptr<ITurboScene> Scene() = 0;

	//	IGameLevel Methods  ------------------------------------------------------------------------------------------------
	virtual void	Initialize() = 0;
	virtual void	Finalize() = 0;

	virtual void	Update(NavigationInfo navInfo) = 0;
//	virtual void	Render() = 0;
};