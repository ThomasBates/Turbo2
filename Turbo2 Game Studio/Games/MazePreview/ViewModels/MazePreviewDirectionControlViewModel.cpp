
#include <pch.h>

#include <MazePreviewDirectionControlViewModel.h>

MazePreviewDirectionControlViewModel::MazePreviewDirectionControlViewModel(
        std::shared_ptr<ITurboControlViewModel> directionViewModel) :
        _directionViewModel(directionViewModel)
{
}
