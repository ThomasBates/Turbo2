
#pragma once

#include "ITurboScene.h"

class ITurboApplicationDX12Renderer
{
public:
	//  ITurboApplicationDX12Renderer Methods  ----------------------------------------------------------------------------
	virtual void	Resize() = 0;
	virtual bool	RenderStaticScene(std::shared_ptr<ITurboScene> staticScene) = 0;
	virtual bool	RenderDynamicScene(std::shared_ptr<ITurboScene> dynamicScene) = 0;
};

