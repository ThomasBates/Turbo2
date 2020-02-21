
#include <pch.h>

#include <MazePreviewMainControlViewModel.h>

MazePreviewMainControlViewModel::MazePreviewMainControlViewModel(
        std::shared_ptr<ITurboControlViewModel> controlViewModel) :
        _controlViewModel(controlViewModel)
{
}
