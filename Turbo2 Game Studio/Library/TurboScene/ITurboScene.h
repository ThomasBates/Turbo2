#pragma once

#include <ITurboSceneObject.h>

namespace Turbo
{
	namespace Scene
	{
		class ITurboScene
		{
		public:
			//  ITurboScene Properties -----------------------------------------------------------------------------------------
			virtual std::vector<std::shared_ptr<ITurboSceneObject>> SceneObjects() = 0;

			virtual std::shared_ptr<ITurboScenePlacement> CameraPlacement() = 0;
			virtual void CameraPlacement(std::shared_ptr<ITurboScenePlacement> cameraPlacement) = 0;

			//  ITurboScene Methods --------------------------------------------------------------------------------------------
			virtual void AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject) = 0;
			//virtual int Render() = 0;
		};
	}
}