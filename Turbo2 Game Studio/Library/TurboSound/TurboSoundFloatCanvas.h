
#pragma once

#include <ITurboSoundCanvas.h>

namespace Turbo
{
    namespace Sound
    {
        class TurboSoundFloatCanvas : public ITurboSoundCanvas
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSoundFloatCanvas();
            TurboSoundFloatCanvas(int sampleRate, int channelCount, int length);
            virtual ~TurboSoundFloatCanvas();

            //  ITurboSoundCanvas Properties ---------------------------------------------------------------------------
            virtual void* Data();
            virtual void Data(void* data);
            virtual int DataSize();

            virtual int SampleSize() { return _sampleSize; }

            virtual int SampleRate() { return _sampleRate; }
            virtual void SampleRate(int sampleRate) { _sampleRate = sampleRate; }

            virtual int ChannelCount() {return _channelCount; }
            virtual void ChannelCount(int channelCount);

            virtual int Length() { return _length; }
            virtual void Length(int length);

            //  ITurboSoundCanvas Methods ------------------------------------------------------------------------------
            virtual void Clear();
            virtual void Normalize(float maxAmplitude);

            virtual int GetSampleAsInt(int sampleIndex, int channelIndex);
            virtual float GetSampleAsFloat(int sampleIndex, int channelIndex);

            virtual void SetSampleAsInt(int sampleIndex, int channelIndex, int sampleValue);
            virtual void SetSampleAsFloat(int sampleIndex, int channelIndex, float sampleValue);

        private:
            float* _data {nullptr};
            int _sampleSize {sizeof(float)};
            int _sampleRate;
            int _channelCount;
            int _length = 0;
            bool _isNormalized {false};

            void NormalizeData();
        };
    }
}
