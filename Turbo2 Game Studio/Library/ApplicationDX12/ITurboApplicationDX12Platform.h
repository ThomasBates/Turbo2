
#pragma once

class ITurboApplicationDX12Platform
{
public:
	//  ITurboApplicationPlatform Methods  -----------------------------------------------------------------------------
	virtual void	Initialize() = 0;
	virtual void	Update() = 0;
	virtual void	Render() = 0;
	virtual void	SaveState() = 0;
	virtual void	LoadState() = 0;
	virtual void	Finalize() = 0;

	virtual void	Resize(float width, float height) = 0;
	virtual void	SetDPI(float logicalDPI) = 0;
	virtual void	SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation) = 0;
	virtual void	ValidateDevice() = 0;

	//  ITurboApplicationPlatform Properties  -----------------------------------------------------------------------------
};

