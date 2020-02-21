
#pragma once

#include <pch.h>

#include <ITurboControlViewModel.h>

using namespace Turbo::View;

class MazePreviewMainControlViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewMainControlViewModel(
            std::shared_ptr<ITurboControlViewModel> controlViewModel);
    virtual ~MazePreviewMainControlViewModel() {}

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<ITurboControlViewModel> ControlViewModel() { return _controlViewModel; }

private:
    std::shared_ptr<ITurboControlViewModel> _controlViewModel;
};
