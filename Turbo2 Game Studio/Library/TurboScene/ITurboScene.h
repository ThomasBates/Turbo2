#pragma once

#include <pch.h>

#include <ITurboSceneLight.h>
#include <ITurboSceneObject.h>
#include <ITurboSceneSprite.h>
#include <ITurboSceneText.h>

namespace Turbo
{
	namespace Scene
	{
		class ITurboScene
		{
		public:
			//  ITurboScene Properties -----------------------------------------------------------------------------------------
			virtual std::vector<std::shared_ptr<ITurboSceneObject>> SceneObjects() = 0;
			virtual std::vector<std::shared_ptr<ITurboSceneSprite>> SceneSprites() = 0;
			virtual std::vector<std::shared_ptr<ITurboSceneText>> SceneTexts() = 0;

			virtual std::shared_ptr<ITurboScenePlacement> CameraPlacement() = 0;
			virtual void CameraPlacement(std::shared_ptr<ITurboScenePlacement> cameraPlacement) = 0;

			virtual bool LightHack() = 0;
			virtual void LightHack(bool lightHack) = 0;

			//  ITurboScene Methods --------------------------------------------------------------------------------------------
			virtual void AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject) = 0;
			virtual void AddSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) = 0;
			virtual void AddSceneText(std::shared_ptr<ITurboSceneText> sceneText) = 0;
		};
	}
}