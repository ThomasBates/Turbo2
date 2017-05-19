#pragma once

#include "pch.h"

class ISoundEffect
{
	virtual void Initialize(
		//IAudioEngine* audioEngine,
		//WaveFormat* waveFormat,
		Platform::Array<byte>^ soundData
		) = 0;
	virtual void PlaySound(float volume) = 0;
};