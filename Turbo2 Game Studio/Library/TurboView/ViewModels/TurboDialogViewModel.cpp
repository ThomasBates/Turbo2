
#include <pch.h>

#include <TurboDialogViewModel.h>
#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>
#include <TurboControlViewModel.h>
#include <TurboSceneNavigationButtonControl.h>

TurboDialogViewModel::TurboDialogViewModel(std::shared_ptr<ITurboGame> game, bool showCancel) :
        _showCancel(showCancel)
{
    _sceneViewModel = std::shared_ptr<TurboSceneViewModel>(new TurboSceneViewModel(game));

    auto okButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Action, 1, 0, 0));
    _okViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(okButton));

    if (_showCancel)
    {
        auto cancelButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Action, 2, 0, 0));
        _cancelViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(cancelButton));
    }
}

void TurboDialogViewModel::Update()
{
    _okAction = _okViewModel->IsActivated();

    if (_showCancel)
        _cancelAction = _cancelViewModel->IsActivated();

    if (_okAction)
        DoCustomSaveData();

    if (_cancelAction)
        DoCustomUndoChanges();
}

void TurboDialogViewModel::IsVisible(bool isVisible)
{
    if (_isVisible == isVisible)
        return;

    _isVisible = isVisible;

    if (isVisible)
        DoCustomLoadData();
}
