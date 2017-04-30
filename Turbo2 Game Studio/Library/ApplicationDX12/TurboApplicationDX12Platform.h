
#pragma once

#include <INavigationController.h>

#include <ITurboApplicationDX12Platform.h>
#include <ITurboApplicationDX12Renderer.h>

namespace Application_DX12
{
	class TurboApplicationDX12Platform : public ITurboApplicationDX12Platform
	{
	public:
		//  Constructors and Destructors  ------------------------------------------------------------------------------
		TurboApplicationDX12Platform(std::shared_ptr<IGameLevel> program);
		~TurboApplicationDX12Platform();

		//  ITurboApplicationDX12Platform Methods  ---------------------------------------------------------------------
		virtual void Initialize();
		virtual void Update();
		virtual void Render();
		virtual void SaveState();
		virtual void LoadState();
		virtual void Finalize();

		virtual void Resize(float width, float height);
		virtual void SetDPI(float logicalDPI);
		virtual void SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation);
		virtual void ValidateDevice();

		//  ITurboApplicationDX12Platform Properties  ------------------------------------------------------------------

	private:
		std::shared_ptr<IGameLevel>						_program;
		std::unique_ptr<INavigationController>			_controller;
		std::shared_ptr<ITurboApplicationDX12Renderer>	_sceneRenderer;

		//  Local Support Methods  -------------------------------------------------------------------------------------
		std::shared_ptr<ITurboApplicationDX12Renderer> GetSceneRenderer();

		void SaveGameState(std::shared_ptr<IApplicationState> programState);
		std::shared_ptr<IApplicationState> LoadGameState();
	};
}

