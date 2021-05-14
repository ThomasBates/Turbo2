
#pragma once

#include <pch.h>

#include <TurboGroupView.h>
#include <ITurboControlView.h>
#include <ViewModels/MazePreviewMotionControlViewModel.h>
#include <ViewModels/TurboToggleControlViewModel.h>

namespace Turbo
{
    namespace View
    {
        class TurboToggleControlView : public TurboGroupView
        {
        public:
            //	Constructors and Destructors -----------------------------------------------------------------------------------
            TurboToggleControlView(
                    std::string name,
                    std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
                    std::shared_ptr<TurboToggleControlViewModel> viewModel);
            virtual ~TurboToggleControlView(){}

            //	ITurboView Methods ---------------------------------------------------------------------------------------------
            virtual void Load();
            virtual void Render();

        protected:
            virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

        private:
            std::shared_ptr<TurboToggleControlViewModel> _viewModel;
            std::shared_ptr<ITurboView> _toggleControlView;
            std::shared_ptr<ITurboSceneTexture> _toggleOffTexture;
            std::shared_ptr<ITurboSceneTexture> _toggleOnTexture;
            std::shared_ptr<ITurboSceneSprite> _toggleSprite;
       };
    }
}