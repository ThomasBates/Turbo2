#pragma once

#include <IPlatformResources.h>

class IMaterial
{
	virtual void RenderSetup(IPlatformResources *platformResources) = 0;
	virtual void SetTexture(/*ITexture *texture*/) = 0;
};