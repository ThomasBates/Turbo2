
#pragma once

#include <pch.h>

#include <TurboSceneViewModel.h>
#include <MazePreviewMainControlViewModel.h>
#include <MazePreviewMotionControlViewModel.h>
#include <MazePreviewDirectionControlViewModel.h>

using namespace Turbo::Game;

class MazePreviewMainViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewMainViewModel(
            std::shared_ptr<TurboSceneViewModel> mainSceneViewModel,
//            std::shared_ptr<MazePreviewHUD1ViewModel> hud1ViewModel,
//            std::shared_ptr<MazePreviewHUD2ViewModel> hud2ViewModel,
            std::shared_ptr<MazePreviewMainControlViewModel> mainControlViewModel,
            std::shared_ptr<MazePreviewMotionControlViewModel> motionControlViewModel,
            std::shared_ptr<MazePreviewDirectionControlViewModel> directionControlViewModel
            );
    virtual ~MazePreviewMainViewModel() {}

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<TurboSceneViewModel> MainSceneViewModel() { return _mainSceneViewModel; }
//    std::shared_ptr<MazePreviewHUD1ViewModel> HUD1ViewModel() { return _hud1ViewModel; }
//    std::shared_ptr<MazePreviewHUD2ViewModel> HUD2ViewModel() { return _hud2ViewModel; }
    std::shared_ptr<MazePreviewMainControlViewModel> MainControlViewModel() { return _mainControlViewModel; }
    std::shared_ptr<MazePreviewMotionControlViewModel> MotionControlViewModel() { return _motionControlViewModel; }
    std::shared_ptr<MazePreviewDirectionControlViewModel> DirectionControlViewModel() { return _directionControlViewModel; }

private:
    std::shared_ptr<TurboSceneViewModel> _mainSceneViewModel;
//    std::shared_ptr<MazePreviewHUD1ViewModel> _hud1ViewModel;
//    std::shared_ptr<MazePreviewHUD2ViewModel> _hud2ViewModel;
    std::shared_ptr<MazePreviewMainControlViewModel> _mainControlViewModel;
    std::shared_ptr<MazePreviewMotionControlViewModel> _motionControlViewModel;
    std::shared_ptr<MazePreviewDirectionControlViewModel> _directionControlViewModel;
};
