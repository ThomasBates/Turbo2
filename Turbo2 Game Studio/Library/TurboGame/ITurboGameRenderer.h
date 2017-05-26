
#pragma once

#include <ITurboScene.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameRenderer
		{
		public:
			//	ITurboGameRenderer Methods -----------------------------------------------------------------------------
			virtual void UpdateDisplayInformation() = 0;
			virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene) = 0;
			virtual bool RenderScene(std::shared_ptr<ITurboScene> scene) = 0;
		};
	}
}
