
#include <pch.h>

#include <TurboSound16BitCanvas.h>
#include "TurboSound16BitCanvas.h"


using namespace Turbo::Sound;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

TurboSound16BitCanvas::TurboSound16BitCanvas() :
    _data(nullptr),
    _sampleRate(0),
    _channelCount(0),
    _length(0)
{
    Clear();
}

TurboSound16BitCanvas::TurboSound16BitCanvas(int sampleRate, int channelCount, int length) :
    _data(nullptr),
    _sampleRate(sampleRate),
    _channelCount(channelCount),
    _length(length)
{
    Clear();
}

TurboSound16BitCanvas::~TurboSound16BitCanvas()
{
    delete [] _data;
}

//  ITurboSoundCanvas Properties ---------------------------------------------------------------------------------------

void *TurboSound16BitCanvas::Data()
{
    return (void*)_data;
}

void TurboSound16BitCanvas::Data(void *data)
{
    delete[] _data;
    _data = (int16_t*)data;
}

int TurboSound16BitCanvas::DataSize()
{
    return _length * _channelCount * _sampleSize;
}

void TurboSound16BitCanvas::ChannelCount(int channelCount)
{
    if (_channelCount == channelCount)
        return;

    _channelCount = channelCount;

    Clear();
}

void TurboSound16BitCanvas::Length(int length)
{
    if (_length == length)
        return;

    _length = length;

    Clear();
}

//  ITurboSoundCanvas Methods ------------------------------------------------------------------------------------------

void TurboSound16BitCanvas::Clear()
{
    delete [] _data;

    if (_length * _channelCount * _sampleSize == 0)
        return;

    _data = new int16_t[_length * _channelCount];

    memset(_data, 0, (size_t)_length * _channelCount * _sampleSize);
}

void TurboSound16BitCanvas::Normalize(float maxAmplitude)
{

}

int TurboSound16BitCanvas::GetSampleAsInt(int sampleIndex, int channelIndex)
{
    if (_data == nullptr)
        return 0;

    return _data[sampleIndex * _channelCount + channelIndex];
}

float TurboSound16BitCanvas::GetSampleAsFloat(int sampleIndex, int channelIndex)
{
    if (_data == nullptr)
        return 0;

    return _data[sampleIndex * _channelCount + channelIndex];
}

void TurboSound16BitCanvas::SetSampleAsInt(int sampleIndex, int channelIndex, int sampleValue)
{
    if (_data == nullptr)
        return;

    _data[sampleIndex * _channelCount + channelIndex] = (int16_t)sampleValue;
}

void TurboSound16BitCanvas::SetSampleAsFloat(int sampleIndex, int channelIndex, float sampleValue)
{
    if (_data == nullptr)
        return;

    _data[sampleIndex * _channelCount + channelIndex] = (int16_t)sampleValue;
}
