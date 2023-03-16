
#include <pch.h>

#include <MazePreviewHUD2ViewModel.h>
#include <TurboControlViewModel.h>
#include <TurboSceneNavigationButtonControl.h>

MazePreviewHUD2ViewModel::MazePreviewHUD2ViewModel(std::shared_ptr<ITurboGame> game) :
        _game(game)
{
    auto menuButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Action, 1, 0, 0));
    auto infoButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Action, 2, 0, 0));

    _openMenuViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(menuButton));
    _openInfoViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(infoButton));
}

void MazePreviewHUD2ViewModel::Update()
{
    _openMenuAction = _openMenuViewModel->IsActivated();
    _openInfoAction = _openInfoViewModel->IsActivated();

    _signageText = _game->GetSignage();
}
