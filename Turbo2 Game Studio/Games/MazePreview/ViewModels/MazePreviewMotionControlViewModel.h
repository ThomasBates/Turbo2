
#pragma once

#include <pch.h>

#include <ITurboControlViewModel.h>

using namespace Turbo::View;

class MazePreviewMotionControlViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewMotionControlViewModel(
            std::shared_ptr<ITurboControlViewModel> forwardViewModel,
            std::shared_ptr<ITurboControlViewModel> backwardViewModel);
    virtual ~MazePreviewMotionControlViewModel() {}

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<ITurboControlViewModel> ForwardViewModel() { return _forwardViewModel; }
    std::shared_ptr<ITurboControlViewModel> BackwardViewModel() { return _backwardViewModel; }

private:
    std::shared_ptr<ITurboControlViewModel> _forwardViewModel;
    std::shared_ptr<ITurboControlViewModel> _backwardViewModel;
};
