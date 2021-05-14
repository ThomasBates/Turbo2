
#pragma once

#include <pch.h>

#include <ITurboControlViewModel.h>

using namespace Turbo::Scene;

namespace Turbo
{
    namespace View
    {
        class TurboToggleControlViewModel
        {
        public:
            //  Constructors and Destructors ---------------------------------------------------------------------------
            TurboToggleControlViewModel();
            virtual ~TurboToggleControlViewModel() {}

            //  Public Methods -----------------------------------------------------------------------------------------
            void Update();

            //  Public Properties --------------------------------------------------------------------------------------
            std::shared_ptr<ITurboControlViewModel> ToggleViewModel() { return _toggleViewModel; }

            bool ToggleOn() { return _toggleOn; }
            void ToggleOn(bool toggleOn) { _toggleOn = toggleOn; }

        private:
            std::shared_ptr<ITurboControlViewModel> _toggleViewModel;

            bool _toggleOn = false;
            bool _lastToggleButtonActive = false;
        };
    }
}