
#pragma once

#include "ITurboScene.h"

class ITurboApplicationRenderer
{
public:
	//  ITurboApplicationRenderer Methods  -----------------------------------------------------------------------------
	virtual void	Initialize() = 0;
	virtual void	Resize() = 0;
	virtual void	BeginDraw() = 0;
	virtual void	EndDraw() = 0;
	virtual void	Update() = 0;
	virtual bool	Render() = 0;
	virtual void	Finalize() = 0;

	virtual void	RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject) = 0;

	//  ITurboApplicationRenderer Properties  --------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboSceneObjectPlacement> CameraPlacement() = 0;
	virtual void CameraPlacement(std::shared_ptr<ITurboSceneObjectPlacement> cameraPlacement) = 0;
};

