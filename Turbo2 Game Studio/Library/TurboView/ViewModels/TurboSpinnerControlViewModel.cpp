#include <TurboSceneNavigationButtonControl.h>
#include "TurboSpinnerControlViewModel.h"
#include "TurboControlViewModel.h"

using namespace Turbo::View;

TurboSpinnerControlViewModel::TurboSpinnerControlViewModel()
{
    auto upButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Action, 1, 0, 0));
    _upButtonViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(upButton));
    auto downButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Action, 1, 0, 0));
    _downButtonViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(downButton));
}

void TurboSpinnerControlViewModel::Update()
{
    if (_upButtonViewModel->IsActivated())
    {
        _value = _value + 1;
        if (_value > _maxValue)
            _value = _maxValue;
    }
    if (_downButtonViewModel->IsActivated())
    {
        _value = _value - 1;
        if (_value < _minValue)
            _value = _minValue;
    }
}
