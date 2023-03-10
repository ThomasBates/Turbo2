
#pragma once

#include <pch.h>

#include <ITurboControlViewModel.h>

using namespace Turbo::Scene;

namespace Turbo
{
    namespace View
    {
        class TurboSpinnerControlViewModel
        {
        public:
            //  Constructors and Destructors ---------------------------------------------------------------------------
            TurboSpinnerControlViewModel();
            virtual ~TurboSpinnerControlViewModel() {}

            //  Public Methods -----------------------------------------------------------------------------------------
            void Update();

            //  Public Properties --------------------------------------------------------------------------------------
            std::shared_ptr<ITurboControlViewModel> DownButtonViewModel() { return _downButtonViewModel; }
            std::shared_ptr<ITurboControlViewModel> UpButtonViewModel() { return _upButtonViewModel; }

            int GetValue() { return _value; }
            void SetValue(int value) { _value = value; }
            int GetMinValue() { return _minValue; }
            void SetMinValue(int minValue) { _minValue = minValue; }
            int GetMaxValue() { return _maxValue; }
            void SetMaxValue(int maxValue) { _maxValue = maxValue; }

        private:
            std::shared_ptr<ITurboControlViewModel> _upButtonViewModel;
            std::shared_ptr<ITurboControlViewModel> _downButtonViewModel;

            int _minValue = INT_MIN;
            int _maxValue = INT_MAX;
            int _value = 0;
        };
    }
}