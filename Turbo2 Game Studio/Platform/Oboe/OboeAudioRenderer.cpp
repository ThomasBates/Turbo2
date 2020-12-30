
#include <pch.h>

#include <jni.h>

#include <TurboSoundFloatCanvas.h>
#include <TurboWavFormat.h>

#include <OboeAudioRenderer.h>
#include <OboeAudioLoopingTrack.h>
#include <OboeAudioSimpleTrack.h>
#include <OboeAudioTestTrack.h>

#include <OboeAudioTypes.h>
#include <TurboCoreHelpers.h>


using namespace Turbo::Core;
using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::Oboe;
using namespace Turbo::Sound;

//  https://github.com/google/oboe/blob/master/docs/GettingStarted.md

#pragma region Constructors and Destructors ----------------------------------------------------------------------------

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
OboeAudioRenderer::OboeAudioRenderer(
    std::shared_ptr<ITurboDebug> debug,
    std::shared_ptr<ITurboGameIOService> ioService) :
    _debug(move(debug)),
    _ioService(move(ioService))
{
}

OboeAudioRenderer::~OboeAudioRenderer()
{
    Reset();
}

#pragma endregion Constructors and Destructors -------------------------------------------------------------------------
#pragma region ITurboGameRenderer Methods ------------------------------------------------------------------------------

void OboeAudioRenderer::InitializeLoading()
{
    OpenStream();
}

void OboeAudioRenderer::FinalizeLoading()
{

}

void OboeAudioRenderer::InitializeRendering()
{

}

void OboeAudioRenderer::FinalizeRendering()
{

}

void OboeAudioRenderer::Reset()
{
    CloseStream();
}

#pragma endregion ITurboGameRenderer Methods ---------------------------------------------------------------------------
#pragma region ITurboViewRendererAccess Methods ------------------------------------------------------------------------

void OboeAudioRenderer::LoadScene(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
        return;

    for (auto& sceneObject : scene->SceneObjects())
    {
        LoadSceneObject(sceneObject);
        LoadChildSceneObjects(sceneObject);
    }
}

void OboeAudioRenderer::LoadSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    if (sceneObject == nullptr)
        return;

    LoadSceneSound(sceneObject->HitSound());
}

void OboeAudioRenderer::LoadChildSceneObjects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    if (sceneObject == nullptr)
        return;

    //	Iterate over child scene objects. Call this method recursively.
    for (auto &childSceneObject : sceneObject->ChildSceneObjects())
    {
        LoadSceneObject(childSceneObject);
        LoadChildSceneObjects(childSceneObject);
    }
}

void OboeAudioRenderer::LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound)
{
    InternalLoadSceneSound(sceneSound);
}

void OboeAudioRenderer::LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground)
{
    InternalLoadSceneSound(sceneBackground);
}

void OboeAudioRenderer::RenderScene(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
        return;

    for (auto& sceneObject : scene->SceneObjects())
    {
        RenderSceneObject(sceneObject);
        RenderChildSceneObjects(sceneObject);
    }
}

void OboeAudioRenderer::RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    if (sceneObject == nullptr)
        return;

    RenderSceneSound(sceneObject->HitSound());
//    RenderSceneBackground(sceneObject->HitSound());
}

void OboeAudioRenderer::RenderChildSceneObjects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    if (sceneObject == nullptr)
        return;

    //	Iterate over child scene objects. Call this method recursively.
    for (auto &childSceneObject : sceneObject->ChildSceneObjects())
    {
        RenderSceneObject(childSceneObject);
        RenderChildSceneObjects(childSceneObject);
    }
}

void OboeAudioRenderer::RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound)
{
    auto canvas = FindSoundCanvas(sceneSound);
    if (canvas == nullptr)
        return;

    auto track = std::shared_ptr<IOboeAudioTrack>(new OboeAudioSimpleTrack(_debug, canvas));
    _audioTracks.push_back(track);
}

void OboeAudioRenderer::RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground)
{
    auto canvas = FindSoundCanvas(sceneBackground);
    if (canvas == nullptr)
        return;

    auto track = std::shared_ptr<IOboeAudioTrack>(new OboeAudioLoopingTrack(canvas));
    _audioTracks.push_back(track);
}

#pragma endregion ITurboViewRendererAccess Methods ---------------------------------------------------------------------
#pragma region AudioStreamCallback Methods -----------------------------------------------------------------------------

DataCallbackResult OboeAudioRenderer::onAudioReady(AudioStream *audioStream, void *audioData, int32_t numFrames)
{
    auto channelCount = audioStream->getChannelCount();

    auto outputBuffer = static_cast<float *>(audioData);


//    for (int i = 0; i < numFrames; ++i) {
//        float sampleValue = kAmplitude *20* sinf(mPhase);
//        for (int j = 0; j < kChannelCount; j++) {
//            outputBuffer[i * kChannelCount + j] = sampleValue;
//        }
//        mPhase += mPhaseIncrement;
//        if (mPhase >= kTwoPi) mPhase -= kTwoPi;
//    }
//    return oboe::DataCallbackResult::Continue;




    // Zero out the incoming container array
    memset(outputBuffer, 0, sizeof(float) * numFrames * channelCount);

    if (_audioTracks.empty())
        return DataCallbackResult::Continue;



//    for (auto& track : _audioTracks)
//    {
//        track->RenderAudio(outputBuffer, numFrames);
//        if (!track->IsPlaying())
//            _audioTracks.remove(track);
//        break;
//    }
//    return oboe::DataCallbackResult::Continue;



    //_debug->Send(debugDebug, debugAudio) << "Playing " << numFrames << " frame(s) of " << _audioTracks.size() << " track(s)\n";

    float mixingBuffer[kBufferSize];

    std::vector<std::shared_ptr<IOboeAudioTrack>> finishedTracks;

    int trackIndex = 0;
    for (auto& track : _audioTracks)
    {
        memset(mixingBuffer, 0, sizeof(float) * numFrames * channelCount);
        track->RenderAudio(mixingBuffer, numFrames);

//        if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio))
//        {
//            auto debug = new std::ostringstream[channelCount];
//            for (int targetIndex = 0; targetIndex < numFrames; ++targetIndex)
//                for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
//                    debug[channelIndex] << " " << mixingBuffer[(targetIndex * channelCount) + channelIndex];
//            for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
//                _debug->Send(debugVerbose, debugAudio) << "Track" << trackIndex + 1 << ": Channel " << channelIndex + 1 << ":" << debug[channelIndex].str() << "\n";
//        }

        for (int j = 0; j < numFrames * channelCount; ++j)
            outputBuffer[j] += mixingBuffer[j];

        if (!track->IsPlaying())
            finishedTracks.push_back(track);

        trackIndex++;
    }

    auto size = _audioTracks.size();
    for (int j = 0; j < numFrames * channelCount; ++j)
        outputBuffer[j] /= size;

//    if (_debug->CategoryEnabled(TurboDebugCategory::debugAudio))
//    {
//        auto debug = new std::ostringstream[channelCount];
//        for (int targetIndex = 0; targetIndex < numFrames; ++targetIndex)
//            for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
//                debug[channelIndex] << " " << outputBuffer[(targetIndex * channelCount) + channelIndex];
//        for (int channelIndex = 0; channelIndex < channelCount; ++channelIndex)
//            _debug->Send(debugVerbose, debugAudio) << "Output: Channel " << channelIndex + 1 << ":" << debug[channelIndex].str() << "\n";
//    }

    for (auto& finishedTrack : finishedTracks)
    {
        _audioTracks.remove(finishedTrack);

        _debug->Send(debugDebug, debugAudio) << "Track finished\n";
    }
    finishedTracks.clear();

    return DataCallbackResult::Continue;
}

void OboeAudioRenderer::onErrorAfterClose(AudioStream *oboeStream, Result error)
{
    AudioStreamCallback::onErrorAfterClose(oboeStream, error);
    Reset();
}

#pragma endregion AudioStreamCallback Methods --------------------------------------------------------------------------
#pragma region Local Support Methods -----------------------------------------------------------------------------------

void OboeAudioRenderer::OpenStream()
{
    if (_audioStream != nullptr)
        return;

    // Create an audio stream
    AudioStreamBuilder builder;
    builder.setCallback(this);
    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(SharingMode::Exclusive);
    builder.setSampleRate(kSampleRate);
    builder.setFormat(oboe::AudioFormat::Float);
    builder.setChannelCount(oboe::ChannelCount::Stereo);

    Result result = builder.openStream(&_audioStream);
    if (result != Result::OK)
    {
        _debug->Send(debugError, debugAudio) << "Failed to open stream. Error: " << convertToText(result) << "\n";
        _audioStream = nullptr;
        return;
    }

    _debug->Send(debugInformation, debugAudio) << "Opened audio stream. sample rate: " << _audioStream->getSampleRate() << ", format: " << convertToText(_audioStream->getFormat()) << ", channels: " << _audioStream->getChannelCount() << "\n";

    // Reduce stream latency by setting the buffer size to a multiple of the burst size
    auto setBufferSizeResult = _audioStream->setBufferSizeInFrames(_audioStream->getFramesPerBurst() * kBufferSizeInBursts);
    if (setBufferSizeResult != Result::OK)
    {
        _debug->Send(debugWarning, debugAudio) << "Failed to set buffer size. Error: " << convertToText(setBufferSizeResult.error()) << "\n";
    }

    result = _audioStream->requestStart();
    if (result != Result::OK)
    {
        _debug->Send(debugError, debugAudio) << "Failed to start stream. Error: " << convertToText(result) << "\n";
        _audioStream = nullptr;
    }

//    _audioTracks.push_back(std::shared_ptr<IOboeAudioTrack>(new OboeAudioTestTrack(WaveType::SinWave, _audioStream->getSampleRate(), _audioStream->getChannelCount(), 440, 0.5F)));
//    _audioTracks.push_back(std::shared_ptr<IOboeAudioTrack>(new OboeAudioTestTrack(WaveType::SinWave, _audioStream->getSampleRate(), _audioStream->getChannelCount(), 660, 0.5F)));
//    _audioTracks.push_back(std::shared_ptr<IOboeAudioTrack>(new OboeAudioLoopingTrack(CreateTestCanvas(440, 0.5F))));
//    _audioTracks.push_back(std::shared_ptr<IOboeAudioTrack>(new OboeAudioLoopingTrack(CreateTestCanvas(660, 0.5F))));
//    _audioTracks.push_back(std::shared_ptr<IOboeAudioTrack>(new OboeAudioLoopingTrack(LoadSoundData("Wall"))));
}

void OboeAudioRenderer::CloseStream()
{
    _audioStream = nullptr;
}

std::shared_ptr<ITurboSoundCanvas> OboeAudioRenderer::InternalLoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound)
{
    if (sceneSound == nullptr)
        return nullptr;

    auto soundName = sceneSound->Name();

    if (soundName.empty())
        return nullptr;

    //  Already loaded this sound? don't reload it.
    if (_sceneSoundData.find(soundName) != _sceneSoundData.end())
        return nullptr;

    auto canvas = LoadSoundData(soundName);

    _sceneSoundData[soundName] = canvas;

    return canvas;
}

std::shared_ptr<ITurboSoundCanvas> OboeAudioRenderer::LoadSoundData(const std::string &soundName)
{
    _debug->Send(debugDebug, debugAudio) << "Loading sound:" << soundName << "\n";

    auto fileData = _ioService->ReadData(ToWString("Sounds/" + soundName + ".wav"));

    if (fileData.empty())
    {
        _debug->Send(debugWarning, debugAudio) << "Failed to load sound: " << soundName << "\n";
        return nullptr;
    }

    auto canvas = std::shared_ptr<ITurboSoundCanvas>(new TurboSoundFloatCanvas());
    auto wavFormat = std::shared_ptr<ITurboSoundFormat>(new TurboWavFormat(_debug, canvas.get(), fileData.data()));

    //wavFormat->RenderToCanvas(_audioStream->getSampleRate(), _audioStream->getChannelCount());
    wavFormat->SampleRate(_audioStream->getSampleRate());
    wavFormat->ChannelCount(_audioStream->getChannelCount());
    wavFormat->RenderToCanvas();

    _debug->Send(debugInformation, debugAudio) << "Loaded sound: " << soundName << ", channels: " << canvas->ChannelCount() << ", length: " << canvas->Length() << "\n";

    return canvas;
}

std::shared_ptr<ITurboSoundCanvas> OboeAudioRenderer::FindSoundCanvas(std::shared_ptr<ITurboSceneSound> sound)
{
    if (sound == nullptr)
        return nullptr;

    if (!sound->PlaySound())
        return nullptr;

    sound->PlaySound(false);

    if (sound->Volume() <= 0.0)
        return nullptr;

    if (_audioStream == nullptr)
        return nullptr;

    //return CreateTestCanvas(660, 0.5F);

    auto soundName = sound->Name();

    //  sound name not found?
    if (_sceneSoundData.find(soundName) == _sceneSoundData.end())
    {
        _debug->Send(debugWarning, debugAudio) << "Sound data not loaded: " << soundName << "\n";
        return nullptr;
    }

    auto canvas = _sceneSoundData[soundName];

    _debug->Send(debugInformation, debugAudio) << "Playing sound: " << soundName << "\n";

    return canvas;
}

std::shared_ptr<ITurboSoundCanvas> OboeAudioRenderer::CreateTestCanvas(float frequency, float amplitude)
{
    auto sampleRate = _audioStream->getSampleRate();
    auto channelCount = _audioStream->getChannelCount();
    auto length = sampleRate;

    auto canvas = std::shared_ptr<ITurboSoundCanvas>(new TurboSoundFloatCanvas(sampleRate, channelCount, length));

    float phase = 0;
    float phaseIncrement = frequency * kTwoPi / sampleRate;
    for (int sampleIndex = 0; sampleIndex < length; sampleIndex++)
    {
        auto sample = amplitude * sinf(phase);
        for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
            canvas->SetSampleAsFloat(sampleIndex, channelIndex, sample);
        phase += phaseIncrement;
    }

    return canvas;
}

#pragma endregion Local Support Methods --------------------------------------------------------------------------------

