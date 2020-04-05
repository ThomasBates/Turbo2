
#pragma once

#include <pch.h>

#include <TurboView.h>
#include <ITurboControlView.h>
#include <ITurboViewRendererAccess.h>
#include <ITurboControlViewModel.h>

namespace Turbo
{
    namespace View
    {
        class TurboControlView : public TurboView, public ITurboControlView
        {
        public:
            //	Constructors and Destructors ---------------------------------------------------------------------------
            TurboControlView(
                    std::string name,
                    std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
                    std::shared_ptr<ITurboControlViewModel> controlViewModel);
            virtual ~TurboControlView(){}

            //	ITurboView Methods -------------------------------------------------------------------------------------
            virtual void Load();
            virtual void Render();

            //	ITurboControlView Properties ---------------------------------------------------------------------------
            virtual bool IsActive();
            virtual void IsActive(bool isActive);

            virtual std::shared_ptr<ITurboControlViewModel> ControlViewModel() { return _controlViewModel; }

            //	ITurboControlView Methods ------------------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z);

        protected:
            virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

        private:
            std::shared_ptr<ITurboControlViewModel> _controlViewModel;
        };
    }
}