
#include <pch.h>

#include <MazePreviewMainViewModel.h>
#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>
#include <TurboControlViewModel.h>

MazePreviewMainViewModel::MazePreviewMainViewModel(const std::shared_ptr<ITurboGame>& game, std::shared_ptr<MazePreviewGameState> gameState)
{
    auto dragControl    = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationPadControl(TurboGameControlType::Look,  0.1F));
    auto dpad1Control   = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationPadControl(TurboGameControlType::Look, -0.5F));
    auto dpad2Control   = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationPadControl(TurboGameControlType::Move,  1.0F, true));
    auto forwardButton  = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Move, 0, -1, 0));
    auto backwardButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Move, 0,  1, 0));

    _mainControlViewModel        = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(dragControl));
    _directionControlViewModel   = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(dpad1Control));
    _strafeControlViewModel      = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(dpad2Control));
    auto forwardButtonViewModel  = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(forwardButton));
    auto backwardButtonViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(backwardButton));

    _sceneViewModel = std::shared_ptr<TurboSceneViewModel>(new TurboSceneViewModel(game));
    _hud1ViewModel = std::shared_ptr<MazePreviewHUD1ViewModel>(new MazePreviewHUD1ViewModel(game, std::move(gameState)));
    _hud2ViewModel = std::shared_ptr<MazePreviewHUD2ViewModel>(new MazePreviewHUD2ViewModel(game));

    _motionControlViewModel    = std::shared_ptr<MazePreviewMotionControlViewModel>(new MazePreviewMotionControlViewModel(forwardButtonViewModel, backwardButtonViewModel));
}

void MazePreviewMainViewModel::Update()
{
    _hud2ViewModel->Update();
}
