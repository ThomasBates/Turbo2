
#pragma once

#include <pch.h>

#include <ITurboControlViewModel.h>

using namespace Turbo::Scene;

namespace Turbo
{
    namespace View
    {
        class TurboControlViewModel : public ITurboControlViewModel
        {
        public:
            //  Constructors and Destructors ---------------------------------------------------------------------------
            TurboControlViewModel(
                    std::shared_ptr<ITurboSceneNavigationControl> control,
                    std::string textureName = "");
            virtual ~TurboControlViewModel() {}

            //	ITurboControlViewModel Properties ----------------------------------------------------------------------
            virtual bool IsActive() { return _control->IsActive(); }
            virtual void IsActive(bool isActive) { _control->IsActive(isActive); }

            virtual std::shared_ptr<ITurboSceneNavigationControl> Control() { return _control; }
            virtual std::shared_ptr<ITurboSceneSprite> Sprite() { return _sprite; }

            //	ITurboControlViewModel Methods -------------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z) { _control->CurrentPoint(x, y, z); }

        private:
            std::shared_ptr<ITurboSceneNavigationControl> _control;
            std::shared_ptr<ITurboSceneSprite> _sprite;
        };
    }
}