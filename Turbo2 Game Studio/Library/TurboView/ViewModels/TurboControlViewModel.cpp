
#include <pch.h>

#include <TurboControlViewModel.h>

using namespace Turbo::Scene;
using namespace Turbo::View;

TurboControlViewModel::TurboControlViewModel(std::shared_ptr <ITurboSceneNavigationControl> control) :
        _control(control)
{
}

bool TurboControlViewModel::IsActivated()
{
    bool isActive = IsActive();
    if (isActive == _lastActive)
        return false;

    _lastActive = isActive;
    return _lastActive;
}