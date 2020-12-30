#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneSound
		{
		public:
			//  ITurboSceneSound Properties ----------------------------------------------------------------------
			virtual std::string Name() = 0;

			virtual float Volume() = 0;
			virtual void Volume(float volume) = 0;

			virtual bool PlaySound() = 0;
			virtual void PlaySound(bool playSound) = 0;
		};
	}
}