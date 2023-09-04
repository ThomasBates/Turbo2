
#include <pch.h>

#include <OboeAudioSimpleTrack.h>

using namespace Turbo::Platform::Oboe;

OboeAudioSimpleTrack::OboeAudioSimpleTrack(std::shared_ptr<ITurboDebug> debug, std::shared_ptr<ITurboSoundCanvas> canvas) :
    _debug(move(debug)),
    _canvas(move(canvas))
{
}

void OboeAudioSimpleTrack::RenderAudio(float *targetData, int32_t targetFrames)
{
    auto channelCount = _canvas->ChannelCount();
    auto sourceFrames = _canvas->Length();
//    auto data = (float*)_canvas->Data();
//
//    auto frames = sourceFrames - _sourceIndex;
//    if (frames > targetFrames)
//        frames = targetFrames;
//
//    memcpy(targetData, data + _sourceIndex * channelCount, (size_t)frames * channelCount);
//    _sourceIndex += frames;
//
//    frames = targetFrames - frames;
//    if (frames > 0)
//    {
//        memset(targetData, 0, (size_t)frames * channelCount);
//        _isPlaying = false;
//    }
//    return;


//    auto frames = sourceFrames - _sourceIndex;

//    auto debug = new std::ostringstream[channelCount];

    memset(targetData, 0, targetFrames * channelCount * sizeof(float));

    for (int targetIndex = 0; targetIndex < targetFrames; ++targetIndex)
    {
        for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
            targetData[(targetIndex * channelCount) + channelIndex] = _canvas->GetSampleAsFloat(_sourceIndex, channelIndex);

        if (++_sourceIndex >= sourceFrames)
        {
            _isPlaying = false;
            //_sourceIndex = 0;
            break;
        }
    }
    return;

    for (int targetIndex = 0; targetIndex < targetFrames; ++targetIndex)
    {
        if (_sourceIndex < sourceFrames)
        {
            for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
            {
                targetData[(targetIndex * channelCount) + channelIndex] = _canvas->GetSampleAsFloat(_sourceIndex, channelIndex);

//                if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio))
//                    debug[channelIndex] << " " << targetData[(targetIndex * channelCount) + channelIndex];
            }
            _sourceIndex++;
        }
        else
        {
            for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
            {
                targetData[(targetIndex * channelCount) + channelIndex] = 0;

//                if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio))
//                    debug[channelIndex] << " " << targetData[(targetIndex * channelCount) + channelIndex];
            }
        }
    }

    if (_sourceIndex >= sourceFrames)
        _isPlaying = false;

//    if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio))
//    {
//        for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
//            _debug->Send(debugVerbose, debugAudio) << "Player: Channel " << channelIndex + 1 << ":" << debug[channelIndex].str() << "\n";
//    }
}
