
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

            bool GetValue() { return _value; }
            void SetValue(bool value) { _value = value; }

        private:
            std::shared_ptr<ITurboControlViewModel> _toggleViewModel;

            bool _value = false;
        };
    }
}