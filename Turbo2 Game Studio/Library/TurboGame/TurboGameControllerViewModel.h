
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGameControllerViewModel.h>
#include <ITurboGameController.h>
#include "ITurboGameControllerViewModel.h"
#include "../TurboCore/TurboDebug/ITurboDebug.h"

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;

namespace Turbo
{
    namespace Game
    {
        class TurboGameControllerViewModel : public ITurboGameControllerViewModel
        {
        public:
            //  Constructors and Destructors -------------------------------------------------------------------------------
            TurboGameControllerViewModel(
                    std::shared_ptr<ITurboDebug> debug,
                    std::shared_ptr<ITurboGameController> controller);
            virtual ~TurboGameControllerViewModel() {}

        private:
            std::shared_ptr<ITurboDebug> _debug;
            std::shared_ptr<ITurboGameController> _controller;

        };
    }
}