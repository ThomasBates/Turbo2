
#pragma once

#include "IGameLevel.h"

class ITurboApplicationDX12Renderer
{
public:
	//  ITurboApplicationDX12Renderer Methods  ----------------------------------------------------------------------------
	virtual void	Resize(float width, float height) = 0;
	virtual void	SetDPI(float logicalDPI) = 0;
	virtual void	SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation) = 0;
	virtual void	ValidateDevice() = 0;
	virtual bool	NeedsReset() = 0;

	virtual bool	LoadLevelResources(std::shared_ptr<IGameLevel> level) = 0;
	virtual bool	RenderLevel(std::shared_ptr<IGameLevel> level) = 0;
};

