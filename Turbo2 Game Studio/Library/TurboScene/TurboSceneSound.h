#pragma once

#include <pch.h>

#include <ITurboSceneSound.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneSound : public ITurboSceneSound
		{
		public:
			//  Constructors -------------------------------------------------------------------------------------------
			TurboSceneSound(std::string name);
			virtual ~TurboSceneSound(){}

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