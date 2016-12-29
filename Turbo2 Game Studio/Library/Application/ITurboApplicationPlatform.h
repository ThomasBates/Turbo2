
#pragma once

#include "INavigationController.h"
#include "IPlatformResources.h"
#include "ITurboSceneObject.h"

class ITurboApplicationPlatform
{
public:
	//  ITurboApplicationPlatform Methods  -----------------------------------------------------------------------------
	virtual int		Initialize() = 0;
	virtual void	SetPlatformResources(std::shared_ptr<IPlatformResources> platformResources) = 0;
	virtual int		Resize(int width, int height) = 0;
	virtual int		BeginDraw() = 0;
	virtual int		EndDraw() = 0;
	virtual int		BeginUpdate() = 0;
	virtual int		EndUpdate() = 0;
	virtual int		BeginRender() = 0;
	virtual int		EndRender() = 0;
	virtual int		Finalize() = 0;

	virtual std::shared_ptr<ITurboSceneObjectMesh> CreateMesh() = 0;
	virtual std::shared_ptr<ITurboSceneObjectTexture> LoadTexture(std::string textureName) = 0;

	virtual void SetTimeStampForFrame() = 0;
	virtual NavigationInfo GetNavigationInfo() = 0;
	virtual void RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject) = 0;

	//  ITurboApplicationPlatform Properties  -----------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboSceneObjectPlacement> CameraPlacement() = 0;
	virtual void CameraPlacement(std::shared_ptr<ITurboSceneObjectPlacement> cameraPlacement) = 0;
};

