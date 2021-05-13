
#include <pch.h>

#include <MazePreviewMenuViewModel.h>
#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>
#include <TurboControlViewModel.h>
#include <TurboSceneNavigationTapControl.h>

MazePreviewMenuViewModel::MazePreviewMenuViewModel(std::shared_ptr<ITurboGame> game)
{
    _sceneViewModel = std::shared_ptr<TurboSceneViewModel>(new TurboSceneViewModel(game));

    auto closeButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationTapControl(TurboGameControlType::Action, 1, 0, 0));
    _closeViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(closeButton, "CloseButton"));
}

void MazePreviewMenuViewModel::Update()
{
    _closeAction = !_lastCloseButtonActive && _closeViewModel->IsActive();
    _lastCloseButtonActive = _closeViewModel->IsActive();
}
