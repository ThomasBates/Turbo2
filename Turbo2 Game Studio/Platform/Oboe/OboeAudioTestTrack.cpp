
#include <pch.h>

#include "OboeAudioTestTrack.h"

using namespace Turbo::Platform::Oboe;

OboeAudioTestTrack::OboeAudioTestTrack(WaveType waveType, int32_t sampleRate, int channelCount, float frequency, float amplitude) :
    _waveType(waveType),
    _sampleRate(sampleRate),
    _channelCount(channelCount),
    _frequency(frequency),
    _amplitude(amplitude)
{
    _phaseIncrement = _frequency * _2Pi / _sampleRate;
}

void OboeAudioTestTrack::RenderAudio(float *audioData, int32_t targetFrames)
{
    switch (_waveType)
    {
        case WaveType::SinWave:
            for (int i = 0; i < targetFrames; ++i)
            {
                float sampleValue = _amplitude * sinf(_phase);

                for (int j = 0; j < _channelCount; j++)
                    audioData[i * _channelCount + j] = sampleValue;

                _phase += _phaseIncrement;
                if (_phase >= _2Pi)
                    _phase -= _2Pi;
            }
            break;

        case WaveType::SquareWave:
            for (int i = 0; i < targetFrames; ++i)
            {
                float sampleValue = _amplitude * sinf(_phase);

                sampleValue = sampleValue > 0 ? 1 : -1;

                for (int j = 0; j < _channelCount; j++)
                    audioData[i * _channelCount + j] = sampleValue;

                _phase += _phaseIncrement;
                if (_phase >= _2Pi)
                    _phase -= _2Pi;
            }
            break;
    }
}
