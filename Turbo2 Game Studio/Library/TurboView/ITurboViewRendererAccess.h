
#pragma once

#include <pch.h>

#include <ITurboScene.h>
#include <ITurboSceneSprite.h>
#include <ITurboSceneText.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace View
	{
		class ITurboViewRendererAccess
		{
		public:
			//	ITurboViewRendererAccess Methods ----------------------------------------------------------------------
			virtual void LoadScene(std::shared_ptr<ITurboScene> scene) = 0;
			virtual void LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) = 0;
			virtual void LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText) = 0;

			virtual void RenderScene(std::shared_ptr<ITurboScene> scene) = 0;
			virtual void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) = 0;
			virtual void RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText) = 0;
		};
	}
}
