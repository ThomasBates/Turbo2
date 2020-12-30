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
//                virtual std::shared_ptr<ITurboViewRendererAccess> RendererAccess() { return std::shared_ptr<ITurboViewRendererAccess>(this); }
                std::shared_ptr<ITurboViewRendererAccess> RendererAccess() override { return std::shared_ptr<ITurboViewRendererAccess>(this); }

                //	ITurboGameRenderer Methods -------------------------------------------------------------------------
//				virtual void InitializeLoading();
//				virtual void FinalizeLoading();
//				virtual void InitializeRendering();
//				virtual void FinalizeRendering();
//				virtual void Reset();

                void InitializeLoading() override;
                void FinalizeLoading() override;
                void InitializeRendering() override;
                void FinalizeRendering() override;
                void Reset() override;

                //	ITurboViewRendererAccess Methods -------------------------------------------------------------------
//                virtual void LoadScene(std::shared_ptr<ITurboScene> scene);
//                virtual void LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) {}
//                virtual void LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText) {}
//
//                virtual void LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound);
//                virtual void LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground);
//
//                virtual void RenderScene(std::shared_ptr<ITurboScene> scene);
//                virtual void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) {}
//                virtual void RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText) {}
//
//                virtual void RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound);
//                virtual void RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground);

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
//                virtual DataCallbackResult onAudioReady(AudioStream *audioStream, void *audioData, int32_t numFrames);
//                virtual void onErrorAfterClose(AudioStream *oboeStream, Result error);

                DataCallbackResult onAudioReady(AudioStream *audioStream, void *audioData, int32_t numFrames) override;
                void onErrorAfterClose(AudioStream *oboeStream, Result error) override;

            private:
                std::shared_ptr<ITurboDebug> _debug;
                std::shared_ptr<ITurboGameIOService> _ioService;

                AudioStream *_audioStream = nullptr;
                std::map<std::string, std::shared_ptr<ITurboSoundCanvas>> _sceneSoundData;
                std::list<std::shared_ptr<IOboeAudioTrack>> _audioTracks {};



//                OboeAudioMixer _mixer;


                static int constexpr kChannelCount = 2;
                static int constexpr kSampleRate = 44100; // 48000;
                // Wave params, these could be instance variables in order to modify at runtime
                static float constexpr kAmplitude = 0.5f;
                static float constexpr kFrequency = 660;
                static float constexpr kPI = M_PI;
                static float constexpr kTwoPi = kPI * 2;
                static double constexpr mPhaseIncrement = kFrequency * kTwoPi / (double) kSampleRate;
                // Keeps track of where the wave is
//                float mPhase = 0.0;

                //	Local Support Methods --------------------------------------------------------------------------------------
                void OpenStream();
                void CloseStream();

                std::shared_ptr<ITurboSoundCanvas> InternalLoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound);
                std::shared_ptr<ITurboSoundCanvas> LoadSoundData(const std::string &soundName);

                std::shared_ptr<ITurboSoundCanvas> FindSoundCanvas(std::shared_ptr<ITurboSceneSound> sound);

                void LoadSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
                void LoadChildSceneObjects(std::shared_ptr<ITurboSceneObject> sceneObject);

                void RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);

                void RenderChildSceneObjects(std::shared_ptr<ITurboSceneObject> sceneObject);

                std::shared_ptr<ITurboSoundCanvas> CreateTestCanvas(float frequency, float amplitude);
            };
        }
    }
}
