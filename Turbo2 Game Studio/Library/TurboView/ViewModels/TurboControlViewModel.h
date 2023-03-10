
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
            TurboControlViewModel(std::shared_ptr<ITurboSceneNavigationControl> control);
            virtual ~TurboControlViewModel() {}

            //	ITurboControlViewModel Properties ----------------------------------------------------------------------
            virtual bool IsActive() { return _control->IsActive(); }
            virtual void IsActive(bool isActive) { _control->IsActive(isActive); }
            virtual bool IsActivated();

            virtual std::shared_ptr<ITurboSceneNavigationControl> Control() { return _control; }

            //	ITurboControlViewModel Methods -------------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z) { _control->CurrentPoint(x, y, z); }

        private:
            std::shared_ptr<ITurboSceneNavigationControl> _control;
            bool _lastActive = false;
        };
    }
}