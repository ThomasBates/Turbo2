
#include <pch.h>

#include <MazePreviewMotionControlViewModel.h>

MazePreviewMotionControlViewModel::MazePreviewMotionControlViewModel(
        std::shared_ptr<ITurboControlViewModel> forwardViewModel,
        std::shared_ptr<ITurboControlViewModel> backwardViewModel) :
        _forwardViewModel(forwardViewModel),
        _backwardViewModel(backwardViewModel)
{
}
