
#pragma once

#include <pch.h>

#include <IOboeAudioTrack.h>

namespace Turbo
{
    namespace Platform
    {
        namespace Oboe
        {
            enum WaveType
            {
                SinWave,
                SquareWave
            };

            class OboeAudioTestTrack : public IOboeAudioTrack
            {
            public:
                OboeAudioTestTrack(WaveType waveType, int sampleRate, int channelCount, float frequency, float amplitude);
                virtual ~OboeAudioTestTrack() {}

                //  IOboeAudioTrack Properties -------------------------------------------------------------------------
                virtual bool IsPlaying() { return true; }

                //  IOboeAudioTrack Methods ----------------------------------------------------------------------------
                virtual void RenderAudio(float *audioData, int32_t targetFrames);

            private:
                WaveType _waveType;
                int _sampleRate;
                int _channelCount;
                float _frequency;
                float _amplitude;

                float _phase {0};
                float _phaseIncrement;

                float _2Pi = (float)M_2_PI;
            };
        }
    }
}
