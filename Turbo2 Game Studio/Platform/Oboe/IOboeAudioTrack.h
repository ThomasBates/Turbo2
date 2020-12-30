
#pragma once

#include <pch.h>

namespace Turbo
{
    namespace Platform
    {
        namespace Oboe
        {
            class IOboeAudioTrack
            {
            public:
                //  IOboeAudioTrack Properties -------------------------------------------------------------------------
                virtual bool IsPlaying() = 0;

                //  IOboeAudioTrack Methods ----------------------------------------------------------------------------
                virtual void RenderAudio(float *targetData, int32_t targetFrames) = 0;
            };
        }
    }
}
