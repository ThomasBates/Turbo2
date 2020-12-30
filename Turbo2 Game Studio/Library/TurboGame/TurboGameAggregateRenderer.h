
#pragma once

#include <ITurboGameRenderer.h>
#include <ITurboViewRendererAccess.h>

using namespace Turbo::Game;
using namespace Turbo::View;

namespace Turbo
{
    namespace Platform
    {
        namespace OpenGLES
        {
            class TurboGameAggregateRenderer : public ITurboGameRenderer, public ITurboViewRendererAccess
            {
            public:
                //	Constructors ---------------------------------------------------------------------------------------
                TurboGameAggregateRenderer(std::initializer_list<std::shared_ptr<ITurboGameRenderer>> initializer_list);
                virtual ~TurboGameAggregateRenderer() {}

                //	ITurboGameRenderer Properties ----------------------------------------------------------------------
                virtual std::shared_ptr<ITurboViewRendererAccess> RendererAccess() { return std::shared_ptr<ITurboViewRendererAccess>(this); }

                //	ITurboGameRenderer Methods -------------------------------------------------------------------------
                virtual void InitializeLoading();
                virtual void FinalizeLoading();
                virtual void InitializeRendering();
                virtual void FinalizeRendering();
                virtual void Reset();

                //	ITurboViewRendererAccess Methods -------------------------------------------------------------------
                virtual void LoadScene(std::shared_ptr<ITurboScene> scene);
                virtual void LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite);
                virtual void LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText);

                virtual void LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSoundEffect);
                virtual void LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground);

                virtual void RenderScene(std::shared_ptr<ITurboScene> scene);
                virtual void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite);
                virtual void RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText);

                virtual void RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSoundEffect);
                virtual void RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground);

            private:
                //  Private Fields  ------------------------------------------------------------------------------------
                std::vector<std::shared_ptr<ITurboGameRenderer>> _renderers;
                std::vector<std::shared_ptr<ITurboViewRendererAccess>> _rendererAccess;
            };
        }
    }
}
