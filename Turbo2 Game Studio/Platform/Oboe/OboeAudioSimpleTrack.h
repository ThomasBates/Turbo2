
#pragma once

#include <pch.h>

#include <IOboeAudioTrack.h>
#include <ITurboSoundCanvas.h>
#include <ITurboDebug.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Sound;

namespace Turbo
{
    namespace Platform
    {
        namespace Oboe
        {
            class OboeAudioSimpleTrack : public IOboeAudioTrack
            {

            public:
                OboeAudioSimpleTrack(
                        std::shared_ptr<ITurboDebug> debug,
                        std::shared_ptr<ITurboSoundCanvas> canvas);
                virtual ~OboeAudioSimpleTrack() {}

                //  IOboeAudioTrack Properties -------------------------------------------------------------------------
                virtual bool IsPlaying() { return _isPlaying; }

                //  IOboeAudioTrack Methods ----------------------------------------------------------------------------
                virtual void RenderAudio(float *audioData, int32_t targetFrames);

            private:
                std::shared_ptr<ITurboDebug> _debug;
                std::shared_ptr<ITurboSoundCanvas> _canvas;

                std::atomic<bool> _isPlaying { true };

                int32_t _sourceIndex = 0;
            };
        }
    }
}
