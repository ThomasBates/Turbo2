
#pragma once

#include <pch.h>

#include <ITurboSoundCanvas.h>

namespace Turbo
{
    namespace Sound
    {
        class ITurboSoundFormat
        {
        public:
            //  ITurboImage Properties ---------------------------------------------------------------------------------
            virtual ITurboSoundCanvas *Canvas() = 0;
            virtual void Canvas(ITurboSoundCanvas *canvas) = 0;

            virtual void* Data() = 0;
            virtual void Data(void* data) = 0;

            virtual int SampleRate() = 0;
            virtual void SampleRate(int sampleRate) = 0;

            virtual int ChannelCount() = 0;
            virtual void ChannelCount(int channelCount) = 0;

            virtual int Length() = 0;
            virtual void Length(int length) = 0;

            //  ITurboImage Methods ------------------------------------------------------------------------------------
            virtual void RenderToCanvas(int sampleRate = 0, int channelCount = 0, int length = 0) = 0;
        };
    }
}
