
#pragma once

#include <pch.h>

#include <TurboGroupView.h>
#include <ITurboControlView.h>
#include <ViewModels/MazePreviewMotionControlViewModel.h>
#include <ViewModels/TurboSpinnerControlViewModel.h>

namespace Turbo
{
    namespace View
    {
        class TurboSpinnerControlView : public TurboGroupView
        {
        public:
            //	Constructors and Destructors -----------------------------------------------------------------------------------
            TurboSpinnerControlView(
                    std::string name,
                    std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
                    std::shared_ptr<TurboSpinnerControlViewModel> viewModel);
            virtual ~TurboSpinnerControlView(){}

            //	ITurboView Methods ---------------------------------------------------------------------------------------------
            virtual void Load();
            virtual void Render();

        protected:
            virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

        private:
            std::shared_ptr<TurboSpinnerControlViewModel> _viewModel;
            std::shared_ptr<ITurboView> _upButtonControlView;
            std::shared_ptr<ITurboView> _downButtonControlView;
            std::shared_ptr<ITurboSceneText> _valueText;
            std::shared_ptr<ITurboSceneTexture> _upButtonTexture;
            std::shared_ptr<ITurboSceneTexture> _downButtonTexture;
            std::shared_ptr<ITurboSceneSprite> _upButtonSprite;
            std::shared_ptr<ITurboSceneSprite> _downButtonSprite;
       };
    }
}