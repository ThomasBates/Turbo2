
#include <pch.h>

#include <TurboWavFormat.h>

using namespace Turbo::Sound;

//  Constructors -------------------------------------------------------------------------------------------------------

TurboWavFormat::TurboWavFormat(std::shared_ptr<ITurboDebug> debug, ITurboSoundCanvas *canvas, void *data) :
    _debug(move(debug))
{
    Canvas(canvas);
    Data(data);
}

//  ITurboSoundFormat Properties ---------------------------------------------------------------------------------------

void TurboWavFormat::Data(void* data)
{
    _wavFile = nullptr;
    _data = (unsigned char*)data;
    if (_data)
        _wavFile = (WavFile*)_data;
}

//  ITurboSoundFormat Methods ------------------------------------------------------------------------------------------

void TurboWavFormat::RenderToCanvas(int sampleRate, int channelCount, int length)
{
    if (!_canvas || !_wavFile)
        return;

    if (sampleRate > 0)
        _sampleRate = sampleRate;
    if (channelCount > 0)
        _channelCount = channelCount;
    if (length > 0)
        _length = length;

    int sourceSampleRate = _wavFile->fmtChunk.sampleRate;
    int sourceChannelCount = _wavFile->fmtChunk.channels;
    int sourceLength = _wavFile->dataChunk.header.chunkSize
                     / _wavFile->fmtChunk.blockSize;

    _debug->Send(debugDebug, debugAudio) << "TurboWavFormat: source: sample rate = " << sourceSampleRate << ", channels = " << sourceChannelCount << ", length = " << sourceLength << "\n";

    int targetSampleRate;
    if (sampleRate > 0)
        targetSampleRate = sampleRate;
    else if (_sampleRate > 0)
        targetSampleRate = _sampleRate;
    else if (length > 0)
        targetSampleRate = (int)(sourceSampleRate * (float)length / sourceLength);
    else if (_length > 0)
        targetSampleRate = (int)(sourceSampleRate * (float)_length / sourceLength);
    else
        targetSampleRate = sourceSampleRate;

    int targetChannelCount;
    if (channelCount > 0)
        targetChannelCount = channelCount;
    else if (_channelCount > 0)
        targetChannelCount = _channelCount;
    else
        targetChannelCount = sourceChannelCount;

    int targetLength;
    if (length > 0)
        targetLength = length;
    else if (_length > 0)
        targetLength = _length;
    else if (sampleRate > 0)
        targetLength = (int)(sourceLength * (float)sampleRate / sourceSampleRate);
    else if (_sampleRate > 0)
        targetLength = (int)(sourceLength * (float)_sampleRate / sourceSampleRate);
    else
        targetLength = sourceLength;

    _debug->Send(debugDebug, debugAudio) << "TurboWavFormat: target: sample rate = " << targetSampleRate << ", channels = " << targetChannelCount << ", length = " << targetLength << "\n";
    _debug->Send(debugDebug, debugAudio) << "TurboWavFormat: canvas: sample rate = " << _canvas->SampleRate() << ", channels = " << _canvas->ChannelCount() << ", length = " << _canvas->Length() << "\n";

    if (targetSampleRate   != _canvas->SampleRate() ||
        targetChannelCount != _canvas->ChannelCount() ||
        targetLength       != _canvas->Length())
    {
        _canvas->SampleRate(targetSampleRate);
        _canvas->ChannelCount(targetChannelCount);
        _canvas->Length(targetLength);

        if (targetSampleRate   == sourceSampleRate &&
            targetChannelCount == sourceChannelCount &&
            targetLength       == sourceLength)
        {
            RenderSimple();
        }
        else
        {
            RenderScale();
        }

        _canvas->Normalize(0.5F);
    }
}

//  Private Methods ----------------------------------------------------------------------------------------------------

void TurboWavFormat::RenderSimple()
{
    _canvas->Clear();

    unsigned char *sampleData = &(_wavFile->dataChunk.dataStart);
    int sampleSize = _wavFile->fmtChunk.sampleSize / 8;
    auto sampleCount = _canvas->Length();
    auto channelCount = _canvas->ChannelCount();

    for (int sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++)
    for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
    {
        switch (sampleSize)
        {
            case 1:
            {
                auto sample = *(int8_t *)(sampleData + sampleSize * (sampleIndex * channelCount + channelIndex));
                _canvas->SetSampleAsInt(sampleIndex, channelIndex, sample);

                if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio) && ((sampleIndex < 100) || (sampleIndex > sampleCount - 100)))
                {
                    std::ostringstream hex;
                    hex << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << sample;
                    _debug->Send(debugVerbose, debugAudio)
                        << "TurboWavFormat: sample[" << sampleIndex << ", " << channelIndex << "] = " << sample << " (" << hex.str() << ")\n";
                }
                break;
            }

            case 2:
            {
                auto sample = *(int16_t *)(sampleData + sampleSize * (sampleIndex * channelCount + channelIndex));
                _canvas->SetSampleAsInt(sampleIndex, channelIndex, sample);

                if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio) && ((sampleIndex < 100) || (sampleIndex > sampleCount - 100)))
                {
                    std::ostringstream hex;
                    hex << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << sample;
                    _debug->Send(debugVerbose, debugAudio)
                        << "TurboWavFormat: sample[" << sampleIndex << ", " << channelIndex << "] = " << sample << " (" << hex.str() << ")\n";
                }
                break;
            }

            default:
                break;
        }
    }
}

void TurboWavFormat::RenderScale()
{
    _canvas->Clear();

    unsigned char *sampleData = &(_wavFile->dataChunk.dataStart);
    int sampleSize = _wavFile->fmtChunk.sampleSize / 8;

    int sourceChannelCount = _wavFile->fmtChunk.channels;
    int sourceLength = _wavFile->dataChunk.header.chunkSize
                     / _wavFile->fmtChunk.blockSize;

    int targetChannelCount = _canvas->ChannelCount();
    int targetLength = _canvas->Length();

    float sampleScale = (sourceLength - 1) * 1.0F / (targetLength - 1);

    for (int targetSampleIndex = 0; targetSampleIndex < targetLength; targetSampleIndex++)
    {
        float sourceSampleIndex = targetSampleIndex * sampleScale;
        int sourceSampleLowIndex = (int)floorf(sourceSampleIndex);
        int sourceSampleHighIndex = (int)ceilf(sourceSampleIndex);

        auto samples = new float[sourceChannelCount];

        for (int sourceChannelIndex = 0; sourceChannelIndex < sourceChannelCount; sourceChannelIndex++)
        {
            switch (sampleSize)
            {
                case 1:
                {
                    auto lowSample = *(uint8_t *) (sampleData + sampleSize * (sourceSampleLowIndex * sourceChannelCount + sourceChannelIndex));
                    auto highSample = *(uint8_t *) (sampleData + sampleSize * (sourceSampleHighIndex * sourceChannelCount + sourceChannelIndex));
                    float sample = lowSample - 0x80;
                    if (sourceSampleLowIndex != sourceSampleHighIndex)
                    {
                        sample = lowSample - 0x80 + (sourceSampleIndex - sourceSampleLowIndex) * (float) (highSample - lowSample)
                                              / (sourceSampleHighIndex - sourceSampleLowIndex);
                    }
                    samples[sourceChannelIndex] = sample;

                    if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio) && ((targetSampleIndex < 100) || (targetSampleIndex > targetLength - 100)))
                    {
                        std::ostringstream lowHex;
                        lowHex << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)lowSample;
                        std::ostringstream highHex;
                        highHex << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)highSample;
                        _debug->Send(debugVerbose, debugAudio)
                            << "TurboWavFormat: sample[" << targetSampleIndex << ", " << sourceChannelIndex << "] = " << samples[sourceChannelIndex] << " <--- source[" << sourceSampleLowIndex << ", " << sourceChannelIndex << "] (" << lowHex.str() << ") * source[" << sourceSampleHighIndex << ", " << sourceChannelIndex << "] (" << highHex.str() << ")\n";
                    }
                    break;
                }

                case 2:
                {
                    auto lowSample = *(int16_t *) (sampleData + sampleSize * (sourceSampleLowIndex * sourceChannelCount + sourceChannelIndex));
                    auto highSample = *(int16_t *) (sampleData + sampleSize * (sourceSampleHighIndex * sourceChannelCount + sourceChannelIndex));
                    float sample = lowSample;
                    if (sourceSampleLowIndex != sourceSampleHighIndex)
                    {
                        sample = lowSample + (sourceSampleIndex - sourceSampleLowIndex) * (float) (highSample - lowSample)
                                       / (sourceSampleHighIndex - sourceSampleLowIndex);
                    }
                    samples[sourceChannelIndex] = sample;

                    if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio) && ((targetSampleIndex < 100) || (targetSampleIndex > targetLength - 100)))
                    {
                        std::ostringstream lowHex;
                        lowHex << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << lowSample;
                        std::ostringstream highHex;
                        highHex << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << highSample;
                        _debug->Send(debugVerbose, debugAudio)
                            << "TurboWavFormat: sample[" << targetSampleIndex << ", " << sourceChannelIndex << "] = " << samples[sourceChannelIndex] << " <--- source[" << sourceSampleLowIndex << ", " << sourceChannelIndex << "] (" << lowHex.str() << ") * source[" << sourceSampleHighIndex << ", " << sourceChannelIndex << "] (" << highHex.str() << ")\n";
                    }
                    break;
                }

                default:
                    break;
            }
        }

        float channelScale = sourceChannelCount * 1.0F / targetChannelCount;

        for (int targetChannelIndex = 0; targetChannelIndex < targetChannelCount; targetChannelIndex++)
        {
            float sample = 0;

            float sourceChannelLowIndex = targetChannelIndex * channelScale;
            float sourceChannelHighIndex = (targetChannelIndex + 1) * channelScale;

            for (int sourceChannelIndex = 0; sourceChannelIndex < sourceChannelCount; sourceChannelIndex++)
            {
                if (sourceChannelIndex < (int)floorf(sourceChannelLowIndex))
                    continue;
                if (sourceChannelIndex > (int)floorf(sourceChannelHighIndex))
                    break;

                auto low = sourceChannelLowIndex;
                auto high = sourceChannelHighIndex;
                if (low < sourceChannelIndex)
                    low = sourceChannelIndex;
                if (high > sourceChannelIndex + 1)
                    high = sourceChannelIndex + 1;
                if (high < low)
                    continue;
                sample += samples[sourceChannelIndex] * (high - low);
            }

            _canvas->SetSampleAsFloat(targetSampleIndex, targetChannelIndex, sample);

            if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio) && targetSampleIndex < 256)
            {
                std::ostringstream hex;
                hex << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << sample;
                _debug->Send(debugVerbose, debugAudio)
                    << "TurboWavFormat: sample[" << targetSampleIndex << ", " << targetChannelIndex << "] = " << sample << "\n";
            }
        }
    }
}
