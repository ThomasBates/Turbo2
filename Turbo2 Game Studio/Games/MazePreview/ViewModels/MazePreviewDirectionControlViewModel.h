
#pragma once

#include <pch.h>

#include <ITurboControlViewModel.h>

using namespace Turbo::View;

class MazePreviewDirectionControlViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewDirectionControlViewModel(
            std::shared_ptr<ITurboControlViewModel> directionViewModel);
    virtual ~MazePreviewDirectionControlViewModel() {}

    //  Properties -----------------------------------------------------------------------------------------------------
    std::shared_ptr<ITurboControlViewModel> DirectionViewModel() { return _directionViewModel; }

private:
    std::shared_ptr<ITurboControlViewModel> _directionViewModel;
};
