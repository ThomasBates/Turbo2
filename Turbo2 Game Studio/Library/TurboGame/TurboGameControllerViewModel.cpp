
#include <pch.h>

#include "TurboGameControllerViewModel.h"

TurboGameControllerViewModel::TurboGameControllerViewModel(
        std::shared_ptr <ITurboDebug> debug,
        std::shared_ptr <ITurboGameController> controller) :
        _debug(debug),
        _controller(controller)
{

}
