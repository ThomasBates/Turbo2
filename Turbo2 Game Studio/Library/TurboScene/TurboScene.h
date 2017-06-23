
#pragma once

#include <pch.h>

#include <ITurboScene.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboScene : public ITurboScene
		{
		private:
			std::vector<std::shared_ptr<ITurboSceneObject>>	_sceneObjects;
			std::shared_ptr<ITurboScenePlacement>			_cameraPlacement;
			bool											_lightHack;

		public:
			//  Constructors and Destructors -----------------------------------------------------------------------------------
			TurboScene();
			~TurboScene();

			//  ITurboScene Properties -----------------------------------------------------------------------------------------
			virtual std::vector<std::shared_ptr<ITurboSceneObject>> SceneObjects() { return _sceneObjects; }

			virtual std::shared_ptr<ITurboScenePlacement> CameraPlacement() { return _cameraPlacement; }
			virtual void CameraPlacement(std::shared_ptr<ITurboScenePlacement> cameraPlacement) { _cameraPlacement = cameraPlacement; }

			virtual bool LightHack() { return _lightHack; }
			virtual void LightHack(bool lightHack) { _lightHack = lightHack; }

			//  ITurboScene Methods --------------------------------------------------------------------------------------------
			virtual void AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
		};
	}
}
