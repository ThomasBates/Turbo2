
#pragma once

#include <ITurboGameController.h>
#include <ITurboGameLevel.h>
#include <ITurboGameState.h>

#include <DX12Renderer.h>

namespace Application_DX12
{
	class DX12Platform
	{
	public:
		//  Constructors and Destructors -------------------------------------------------------------------------------
		DX12Platform(std::shared_ptr<ITurboGameLevel> program);
		~DX12Platform();

		//  Methods ----------------------------------------------------------------------------------------------------
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

	private:
		std::shared_ptr<ITurboGameLevel>		_level;
		std::unique_ptr<ITurboGameController>	_controller;
		std::shared_ptr<DX12Renderer>			_renderer;

		//  Local Support Methods --------------------------------------------------------------------------------------
		std::shared_ptr<DX12Renderer> GetSceneRenderer();

		void SaveGameState(std::shared_ptr<ITurboGameState> programState);
		std::shared_ptr<ITurboGameState> LoadGameState();
	};
}

