
#pragma once

#include <pch.h>

#include <TurboSceneViewModel.h>
#include <MazePreviewHUD1ViewModel.h>
#include <MazePreviewHUD2ViewModel.h>
#include <MazePreviewMotionControlViewModel.h>

using namespace Turbo::Game;

class MazePreviewMainViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewMainViewModel(const std::shared_ptr<ITurboGame>& game, std::shared_ptr<MazePreviewGameState> gameState);
    virtual ~MazePreviewMainViewModel() {}

    //  Public Methods -------------------------------------------------------------------------------------------------
    void Update();

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<TurboSceneViewModel> MainSceneViewModel() { return _sceneViewModel; }
    std::shared_ptr<MazePreviewHUD1ViewModel> HUD1ViewModel() { return _hud1ViewModel; }
    std::shared_ptr<MazePreviewHUD2ViewModel> HUD2ViewModel() { return _hud2ViewModel; }
    std::shared_ptr<ITurboControlViewModel> MainControlViewModel() { return _mainControlViewModel; }
    std::shared_ptr<MazePreviewMotionControlViewModel> MotionControlViewModel() { return _motionControlViewModel; }
    std::shared_ptr<ITurboControlViewModel> GetStrafeControlViewModel() { return _strafeControlViewModel; }
    std::shared_ptr<ITurboControlViewModel> DirectionControlViewModel() { return _directionControlViewModel; }

    bool IsVisible() { return _isVisible; }
    void IsVisible(bool isVisible) { _isVisible = isVisible; }
    bool OpenMenuAction() { return _hud2ViewModel->OpenMenuAction(); }
    bool OpenInfoAction() { return _hud2ViewModel->OpenInfoAction(); }

private:
    std::shared_ptr<TurboSceneViewModel> _sceneViewModel;
    std::shared_ptr<MazePreviewHUD1ViewModel> _hud1ViewModel;
    std::shared_ptr<MazePreviewHUD2ViewModel> _hud2ViewModel;
    std::shared_ptr<ITurboControlViewModel> _mainControlViewModel;
    std::shared_ptr<MazePreviewMotionControlViewModel> _motionControlViewModel;
    std::shared_ptr<ITurboControlViewModel> _strafeControlViewModel;
    std::shared_ptr<ITurboControlViewModel> _directionControlViewModel;

    bool _isVisible = false;
};


