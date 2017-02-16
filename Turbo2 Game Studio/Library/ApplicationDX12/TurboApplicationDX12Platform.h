
#pragma once

#include <INavigationController.h>

#include <ITurboApplicationDX12Platform.h>
#include <ITurboApplicationDX12Renderer.h>
#include <TurboApplicationDX12Renderer.h>

namespace Application_DX12
{
	class TurboApplicationDX12Platform : public ITurboApplicationDX12Platform
	{
	public:
		//  Constructors and Destructors  ------------------------------------------------------------------------------
		TurboApplicationDX12Platform(std::shared_ptr<IProgram> program);
		~TurboApplicationDX12Platform();

		//  ITurboApplicationDX12Platform Methods  ---------------------------------------------------------------------
		virtual void	Initialize();
		virtual void	Update();
		virtual void	Render();
		virtual void	SaveState();
		virtual void	LoadState();

		//  ITurboApplicationDX12Platform Properties  ------------------------------------------------------------------
		virtual std::shared_ptr<DX::DeviceResources> DeviceResources() { return _deviceResources; }
		virtual void DeviceResources(std::shared_ptr<DX::DeviceResources> deviceResources);

	private:
		std::shared_ptr<IProgram>					_program;
		std::unique_ptr<INavigationController>		_controller;
		std::shared_ptr<DX::DeviceResources>		_deviceResources;
		std::unique_ptr<ITurboApplicationDX12Renderer>	_sceneRenderer;
		std::shared_ptr<ITurboSceneObjectPlacement> _cameraPlacement;
		std::map<std::string, std::string>			_loadedTextures;
		bool _lastRestart;

		//  Local Support Methods  -------------------------------------------------------------------------------------
		void	SaveProgramState(std::shared_ptr<IApplicationState> programState);
		std::shared_ptr<IApplicationState> LoadProgramState();
		void	LoadTextures();
		void	LoadTexture(std::string fileName);
		//virtual IImage *LoadImage(std::string fileName);
	};
}

