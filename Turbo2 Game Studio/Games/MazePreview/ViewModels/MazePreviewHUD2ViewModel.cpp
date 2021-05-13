
#include <pch.h>

#include <MazePreviewHUD2ViewModel.h>
#include <TurboControlViewModel.h>
#include <TurboSceneNavigationTapControl.h>

MazePreviewHUD2ViewModel::MazePreviewHUD2ViewModel(std::shared_ptr<ITurboGame> game) :
        _game(game)
{
    auto menuButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationTapControl(TurboGameControlType::Action, 1, 0, 0));
    auto infoButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationTapControl(TurboGameControlType::Action, 2, 0, 0));

    _menuViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(menuButton, "MenuButton"));
    _infoViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(infoButton, "InfoButton"));
}

void MazePreviewHUD2ViewModel::Update()
{
    _openMenuAction = !_lastMenuButtonActive && _menuViewModel->IsActive();
    _lastMenuButtonActive = _menuViewModel->IsActive();

    _openInfoAction = !_lastInfoButtonActive && _infoViewModel->IsActive();
    _lastInfoButtonActive = _infoViewModel->IsActive();

    _signage = std::string("Welcome"); // _game->Signage();
    _signageText = " " + _signage;
}

std::string MazePreviewHUD2ViewModel::SignageText()
{
    return _signageText;
}
