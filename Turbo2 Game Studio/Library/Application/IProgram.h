
#pragma once

#include "IDeviceResources.h"

class IProgram
{
public:
	virtual LPCWSTR GetTitle() = 0;
	virtual int		Initialize() = 0;
	virtual void	SetDeviceResources(IDeviceResources *deviceResources) = 0;
	virtual int		Resize(int width, int height) = 0;
	virtual int		Update() = 0;
	virtual int		Render() = 0;
	virtual int		SaveState() = 0;
	virtual int		Finalize() = 0;
};