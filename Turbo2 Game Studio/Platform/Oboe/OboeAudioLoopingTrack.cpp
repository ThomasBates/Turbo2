
#include <pch.h>

#include <OboeAudioLoopingTrack.h>

using namespace Turbo::Platform::Oboe;

OboeAudioLoopingTrack::OboeAudioLoopingTrack(std::shared_ptr<ITurboSoundCanvas> canvas) :
    _canvas(move(canvas))
{
}

void OboeAudioLoopingTrack::RenderAudio(float *targetData, int32_t targetFrames)
{
    auto channelCount = _canvas->ChannelCount();
    auto sourceFrames = _canvas->Length();

    for (int targetIndex = 0; targetIndex < targetFrames; ++targetIndex)
    {
        for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
            targetData[(targetIndex * channelCount) + channelIndex] = _canvas->GetSampleAsFloat(_sourceIndex, channelIndex);

        if (++_sourceIndex >= sourceFrames)
            _sourceIndex = 0;
    }
}
