
#include <pch.h>

#include <MazePreviewMainViewModel.h>

MazePreviewMainViewModel::MazePreviewMainViewModel(
        std::shared_ptr<TurboSceneViewModel> mainSceneViewModel,
//        std::shared_ptr<MazePreviewHUD1ViewModel> hud1ViewModel,
//        std::shared_ptr<MazePreviewHUD2ViewModel> hud2ViewModel,
        std::shared_ptr<MazePreviewMainControlViewModel> mainControlViewModel,
        std::shared_ptr<MazePreviewMotionControlViewModel> motionControlViewModel,
        std::shared_ptr<MazePreviewDirectionControlViewModel> directionControlViewModel) :
        _mainSceneViewModel(mainSceneViewModel),
//        _hud1ViewModel(hud1ViewModel),
//        _hud2ViewModel(hud2ViewModel),
        _mainControlViewModel(mainControlViewModel),
        _motionControlViewModel(motionControlViewModel),
        _directionControlViewModel(directionControlViewModel)
{
}
