#pragma once

#include <pch.h>
#include <oboe/Oboe.h>

#include <ITurboGameRenderer.h>
#include <ITurboViewRendererAccess.h>
#include <ITurboView.h>

namespace Turbo
{
    namespace Game
    {
        // This stand-in renderer does nothing.
        class TurboGameNullAudioRenderer :
                public ITurboGameRenderer,
                public ITurboViewRendererAccess
        {
        public:
            TurboGameNullAudioRenderer();
            virtual ~TurboGameNullAudioRenderer();

            //	ITurboGameRenderer Properties ----------------------------------------------------------------------
            std::shared_ptr<ITurboViewRendererAccess> RendererAccess() override { return std::shared_ptr<ITurboViewRendererAccess>(this); }

            //	ITurboGameRenderer Methods -------------------------------------------------------------------------
            void InitializeLoading() override {}
            void FinalizeLoading() override {}
            void InitializeRendering() override {}
            void FinalizeRendering() override {}
            void Reset() override {}

            //	ITurboViewRendererAccess Methods -------------------------------------------------------------------
            void LoadScene(std::shared_ptr<ITurboScene> scene) override {}
            void LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) override {}
            void LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText) override {}

            void LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound) override {}
            void LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) override {}

            void RenderScene(std::shared_ptr<ITurboScene> scene) override {}
            void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) override {}
            void RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText) override {}

            void RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound) override {}
            void RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) override {}

        private:
        };
    }
}
