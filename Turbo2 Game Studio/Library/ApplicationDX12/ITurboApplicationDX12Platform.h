
#pragma once

#include "Common\DeviceResources.h"

#include "INavigationController.h"
#include "ITurboSceneObject.h"

class ITurboApplicationDX12Platform
{
public:
	//  ITurboApplicationPlatform Methods  -----------------------------------------------------------------------------
	virtual void	Initialize() = 0;
	virtual void	Update() = 0;
	virtual void	Render() = 0;
	virtual void	SaveState() = 0;
	virtual void	LoadState() = 0;

	//  ITurboApplicationPlatform Properties  -----------------------------------------------------------------------------
	virtual std::shared_ptr<DX::DeviceResources> DeviceResources() = 0;
	virtual void DeviceResources(std::shared_ptr<DX::DeviceResources> deviceResources) = 0;
};

