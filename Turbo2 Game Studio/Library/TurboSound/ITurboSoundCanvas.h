
#pragma once

#include <pch.h>

namespace Turbo
{
    namespace Sound
    {
        class ITurboSoundCanvas
        {
        public:
            virtual ~ITurboSoundCanvas() {}  //  need this in the interface because we're not using smart pointers here.

            //  ITurboCanvas Properties --------------------------------------------------------------------------------
            virtual void* Data() = 0;
            virtual void Data(void* data) = 0;
            virtual int DataSize() = 0;

            virtual int SampleSize() = 0;

            virtual int SampleRate() = 0;
            virtual void SampleRate(int sampleRate) = 0;

            virtual int ChannelCount() = 0;
            virtual void ChannelCount(int channelCount) = 0;

            virtual int Length() = 0;
            virtual void Length(int length) = 0;

            //  ITurboCanvas Methods -----------------------------------------------------------------------------------
            virtual void Clear() = 0;
            virtual void Normalize(float maxAmplitude) = 0;

            virtual int GetSampleAsInt(int sampleIndex, int channelIndex) = 0;
            virtual float GetSampleAsFloat(int sampleIndex, int channelIndex) = 0;

            virtual void SetSampleAsInt(int sampleIndex, int channelIndex, int sampleValue) = 0;
            virtual void SetSampleAsFloat(int sampleIndex, int channelIndex, float sampleValue) = 0;
        };
    }
}