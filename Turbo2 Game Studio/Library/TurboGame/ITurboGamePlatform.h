
#pragma once

#include <ITurboGameController.h>
#include <ITurboGameLevel.h>
#include <ITurboGameState.h>

#include <DirectX12Renderer.h>

namespace Turbo
{
	namespace Game
	{
		class ITurboGamePlatform
		{
		public:
			//  Methods ----------------------------------------------------------------------------------------------------
			virtual void Initialize() = 0;
			virtual void Update() = 0;
			virtual void Render() = 0;
			virtual void SaveState() = 0;
			virtual void LoadState() = 0;
			virtual void Finalize() = 0;

			/*
			virtual void Resize(float width, float height) = 0;
			virtual void SetDPI(float logicalDPI) = 0;
			virtual void SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation) = 0;
			virtual void ValidateDevice() = 0;
			*/
		};
	}
}
