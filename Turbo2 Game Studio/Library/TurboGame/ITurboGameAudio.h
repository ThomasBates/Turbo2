
#pragma once

#include <ITurboScene.h>

using namespace Turbo::Scene;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameAudio
		{
		public:
			//	ITurboGameAudio Methods --------------------------------------------------------------------------------
			virtual void UpdateAudioInformation() = 0;
			virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene) = 0;
			virtual bool PlaySounds(std::shared_ptr<ITurboScene> scene) = 0;
		};
	}
}
