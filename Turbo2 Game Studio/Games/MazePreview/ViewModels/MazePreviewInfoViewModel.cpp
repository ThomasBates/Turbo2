
#include <pch.h>

#include <MazePreviewInfoViewModel.h>
#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>
#include <TurboControlViewModel.h>
#include <TurboSceneNavigationTapControl.h>

MazePreviewInfoViewModel::MazePreviewInfoViewModel(std::shared_ptr<ITurboGame> game)
{
    _sceneViewModel = std::shared_ptr<TurboSceneViewModel>(new TurboSceneViewModel(game));

    auto closeButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationTapControl(TurboGameControlType::Action, 1, 0, 0));
    _closeViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(closeButton, "CloseButton"));
}

std::string MazePreviewInfoViewModel::InfoText()
{
    return _infoText;
}

void MazePreviewInfoViewModel::Update()
{
    _closeAction = !_lastCloseButtonActive && _closeViewModel->IsActive();
    _lastCloseButtonActive = _closeViewModel->IsActive();
}
