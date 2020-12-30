
#include <pch.h>

#include <TurboSoundFloatCanvas.h>
#include "TurboSoundFloatCanvas.h"


using namespace Turbo::Sound;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

TurboSoundFloatCanvas::TurboSoundFloatCanvas() :
    _data(nullptr),
    _sampleRate(0),
    _channelCount(0),
    _length(0)
{
    Clear();
}

TurboSoundFloatCanvas::TurboSoundFloatCanvas(int sampleRate, int channelCount, int length) :
    _data(nullptr),
    _sampleRate(sampleRate),
    _channelCount(channelCount),
    _length(length)
{
    Clear();
}

TurboSoundFloatCanvas::~TurboSoundFloatCanvas()
{
    delete [] _data;
}

//  ITurboSoundCanvas Properties ---------------------------------------------------------------------------------------

void *TurboSoundFloatCanvas::Data()
{
    return (void*)_data;
}

void TurboSoundFloatCanvas::Data(void *data)
{
    delete[] _data;
    _data = (float*)data;
}

int TurboSoundFloatCanvas::DataSize()
{
    return _length * _channelCount * _sampleSize;
}

void TurboSoundFloatCanvas::ChannelCount(int channelCount)
{
    if (_channelCount == channelCount)
        return;

    _channelCount = channelCount;

    Clear();
}

void TurboSoundFloatCanvas::Length(int length)
{
    if (_length == length)
        return;

    _length = length;

    Clear();
}

//  ITurboSoundCanvas Methods ------------------------------------------------------------------------------------------

void TurboSoundFloatCanvas::Clear()
{
    delete [] _data;
    _isNormalized = false;

    if (_length * _channelCount * _sampleSize == 0)
        return;

    _data = new float[_length * _channelCount];

    memset(_data, 0, (size_t)_length * _channelCount * _sampleSize);
}

void TurboSoundFloatCanvas::Normalize(float maxAmplitude)
{
    if (_data == nullptr)
        return;

    float max = 0;

    for (int i = 0; i < _length * _channelCount; i++)
    {
        auto sample = fabsf(_data[i]);
        if (max < sample)
            max = sample;
    }

    if (max == 0)
        return;

    float factor = maxAmplitude / max;

    for (int i = 0; i < _length * _channelCount; i++)
        _data[i] = _data[i] * factor;

    _isNormalized = true;
}

int TurboSoundFloatCanvas::GetSampleAsInt(int sampleIndex, int channelIndex)
{
    if (_data == nullptr)
        return 0;

    return (int)_data[sampleIndex * _channelCount + channelIndex];
}

float TurboSoundFloatCanvas::GetSampleAsFloat(int sampleIndex, int channelIndex)
{
    if (_data == nullptr)
        return 0;

    return _data[sampleIndex * _channelCount + channelIndex];
}

void TurboSoundFloatCanvas::SetSampleAsInt(int sampleIndex, int channelIndex, int sampleValue)
{
    if (_data == nullptr)
        return;

    _data[sampleIndex * _channelCount + channelIndex] = sampleValue;
}

void TurboSoundFloatCanvas::SetSampleAsFloat(int sampleIndex, int channelIndex, float sampleValue)
{
    if (_data == nullptr)
        return;

    _data[sampleIndex * _channelCount + channelIndex] = sampleValue;
}

//  Private Methods ----------------------------------------------------------------------------------------------------

void TurboSoundFloatCanvas::NormalizeData()
{
    if (_data == nullptr)
        return;

    float max = 0;

    for (int i = 0; i < _length * _channelCount; i++)
    {
        auto sample = fabsf(_data[i]);
        if (max < sample)
            max = sample;
    }

    if (max == 0)
        return;

    float factor = 0.5F / max;

    for (int i = 0; i < _length * _channelCount; i++)
        _data[i] = _data[i] * factor;

    _isNormalized = true;
}
