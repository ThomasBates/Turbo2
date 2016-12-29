
#pragma once

#include "IImage.h"
#include "INavigationController.h"
#include "ITurboApplicationPlatform.h"
#include "ITurboApplicationRenderer.h"

#include "TurboApplicationDX12Renderer.h"

namespace Application_DX12
{
	class TurboApplicationDX12Platform : public ITurboApplicationPlatform
	{
	private:
		std::unique_ptr<INavigationController>		_controller;
		std::unique_ptr<ITurboApplicationRenderer>	_sceneRenderer;
		std::shared_ptr<ITurboSceneObjectPlacement> _cameraPlacement;

		// Rendering loop timer.

		LARGE_INTEGER	_frequency;
		LARGE_INTEGER	_startCount;
		LARGE_INTEGER	_lastCount;

		//  Local Support Methods
		virtual int LoadTextures();
		virtual int LoadTexture(int id, std::string fileName);
		virtual IImage *LoadImage(std::string fileName);

	public:
		//  Constructors and Destructors
		TurboApplicationDX12Platform(std::shared_ptr<IApplication> application);
		~TurboApplicationDX12Platform();

		//  ITurboApplicationPlatform Methods  -----------------------------------------------------------------------------
		virtual int		Initialize();
		virtual void	SetPlatformResources(std::shared_ptr<IPlatformResources> platformResources);
		virtual int		Resize(int width, int height);
		virtual int		BeginDraw();
		virtual int		EndDraw();
		virtual int		BeginUpdate();
		virtual int		EndUpdate();
		virtual int		BeginRender();
		virtual int		EndRender();
		virtual int		Finalize();

		virtual std::shared_ptr<ITurboSceneObjectMesh> CreateMesh();
		virtual std::shared_ptr<ITurboSceneObjectTexture> LoadTexture(std::string textureName);

		virtual void SetTimeStampForFrame();
		virtual NavigationInfo GetNavigationInfo();
		virtual void RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);

		//  ITurboApplicationPlatform Properties  -----------------------------------------------------------------------------
		virtual std::shared_ptr<ITurboSceneObjectPlacement> CameraPlacement();
		virtual void CameraPlacement(std::shared_ptr<ITurboSceneObjectPlacement> cameraPlacement);
	};
}