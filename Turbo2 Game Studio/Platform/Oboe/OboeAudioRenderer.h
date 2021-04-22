#pragma once

#include <pch.h>
#include <oboe/Oboe.h>

#include <ITurboGameRenderer.h>
#include <ITurboViewRendererAccess.h>
#include <ITurboDebug.h>
#include <ITurboGameIOService.h>
#include <ITurboView.h>
#include <ITurboSceneSound.h>
#include <ITurboSoundCanvas.h>

#include <OboeAudioMixer.h>
#include <OboeAudioLoopingTrack.h>
#include <OboeAudioTypes.h>

using namespace oboe;
using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

namespace Turbo 
{
    namespace Platform
    {
        namespace Oboe
        {
            // This sample renderer instantiates a basic rendering pipeline.
            class OboeAudioRenderer :
                    public AudioStreamCallback,
                    public ITurboGameRenderer,
                    public ITurboViewRendererAccess
            {
            public:
                OboeAudioRenderer(
                    std::shared_ptr<ITurboDebug> debug,
                    std::shared_ptr<ITurboGameIOService> ioService);
                virtual ~OboeAudioRenderer();

                //	ITurboGameRenderer Properties ----------------------------------------------------------------------
                std::shared_ptr<ITurboViewRendererAccess> RendererAccess() override { return std::shared_ptr<ITurboViewRendererAccess>(this); }

                //	ITurboGameRenderer Methods -------------------------------------------------------------------------
                void InitializeLoading() override;
                void FinalizeLoading() override;
                void InitializeRendering() override;
                void FinalizeRendering() override;
                void Reset() override;

                //	ITurboViewRendererAccess Methods -------------------------------------------------------------------
                void LoadScene(std::shared_ptr<ITurboScene> scene) override;
                void LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) override {}
                void LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText) override {}

                void LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound) override;
                void LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) override;

                void RenderScene(std::shared_ptr<ITurboScene> scene) override;
                void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) override {}
                void RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText) override {}

                void RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound) override;
                void RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) override;

                //  oboe::AudioStreamCallback Methods ------------------------------------------------------------------
                DataCallbackResult onAudioReady(AudioStream *audioStream, void *audioData, int32_t numFrames) override;
                void onErrorAfterClose(AudioStream *oboeStream, Result error) override;

            private:
                std::shared_ptr<ITurboDebug> _debug;
                std::shared_ptr<ITurboGameIOService> _ioService;

                AudioStream *_audioStream = nullptr;
                std::map<std::string, std::shared_ptr<ITurboSoundCanvas>> _sceneSoundData;
                std::list<std::shared_ptr<IOboeAudioTrack>> _audioTracks {};

                int     _numCallbacks = 0;
                double  _lastCallbackTime = 0;
                double  _totalTime = 0;
                double  _callbackStartTime = 0;
                double  _callbackFinishTime = 0;
                double  _totalDuration = 0;
                double  _totalFrames = 0;

                //	Local Support Methods --------------------------------------------------------------------------------------
                void OpenStream();
                void CloseStream();

                std::shared_ptr<ITurboSoundCanvas> InternalLoadSceneSound(const std::shared_ptr<ITurboSceneSound>& sceneSound);
                std::shared_ptr<ITurboSoundCanvas> LoadSoundData(const std::string &soundName);

                std::shared_ptr<ITurboSoundCanvas> FindSoundCanvas(const std::shared_ptr<ITurboSceneSound>& sound);

                void LoadSceneObject(const std::shared_ptr<ITurboSceneObject>& sceneObject);
                void LoadChildSceneObjects(const std::shared_ptr<ITurboSceneObject>& sceneObject);

                void RenderSceneObject(const std::shared_ptr<ITurboSceneObject>& sceneObject);

                void RenderChildSceneObjects(const std::shared_ptr<ITurboSceneObject>& sceneObject);

                std::shared_ptr<ITurboSoundCanvas> CreateTestCanvas(float frequency, float amplitude);

                void RegisterCallbackStarted(int32_t numFrames);
                void RegisterCallbackFinished();
            };
        }
    }
}
