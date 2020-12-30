
#pragma once

#include <pch.h>

#include <ITurboScene.h>
#include <ITurboSceneSprite.h>
#include <ITurboSceneText.h>
#include <ITurboSceneSound.h>

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

            virtual void LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound) = 0;
			virtual void LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) = 0;

			virtual void RenderScene(std::shared_ptr<ITurboScene> scene) = 0;
			virtual void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) = 0;
			virtual void RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText) = 0;

            virtual void RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound) = 0;
			virtual void RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) = 0;
		};
	}
}
