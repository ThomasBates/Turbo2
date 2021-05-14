#include <TurboSceneNavigationButtonControl.h>
#include "TurboToggleControlViewModel.h"
#include "TurboControlViewModel.h"

using namespace Turbo::View;

TurboToggleControlViewModel::TurboToggleControlViewModel()
{
    auto toggleButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Action, 1, 0, 0));
    _toggleViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(toggleButton, ""));
}

void TurboToggleControlViewModel::Update()
{
    if (!_lastToggleButtonActive && _toggleViewModel->IsActive())
        _toggleOn = !_toggleOn;

    _lastToggleButtonActive = _toggleViewModel->IsActive();
}
