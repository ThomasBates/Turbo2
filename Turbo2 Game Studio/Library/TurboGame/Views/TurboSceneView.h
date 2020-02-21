
#pragma once

#include <pch.h>

#include <TurboView.h>
#include <ITurboViewRendererAccess.h>
#include <TurboSceneViewModel.h>

using namespace Turbo::View;

namespace Turbo
{
    namespace Game
    {
        class TurboSceneView : public TurboView
        {
        public:
            //	Constructors and Destructors ---------------------------------------------------------------------------
            TurboSceneView(
                    std::string name,
                    std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
                    std::shared_ptr<TurboSceneViewModel> viewModel);
            virtual ~TurboSceneView(){}

            //	ITurboView Methods ------------------------------------------------------------------------------
            virtual void Load();
            virtual void Render();

        private:
            std::shared_ptr<ITurboViewRendererAccess> _rendererAccess;
            std::shared_ptr<TurboSceneViewModel> _viewModel;
        };
    }
}