#pragma once

#include <pch.h>

#include <ITurboSceneSoundEffect.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneSoundEffect : public ITurboSceneSoundEffect
		{
		public:
			//  Constructors -------------------------------------------------------------------------------------------
			TurboSceneSoundEffect(std::string name);
			virtual ~TurboSceneSoundEffect(){}

			//  ITurboSceneObjectSoundEffect Properties ----------------------------------------------------------------
			virtual std::string Name() { return _name; }

			virtual float Volume() { return _volume; }
			virtual void Volume(float volume) { _volume = volume; }

			virtual bool PlaySound() { return _playSound; }
			virtual void PlaySound(bool playSound) { _playSound = playSound; }

		private:
			std::string _name;
			float _volume;
			bool _playSound;
		};
	}
}