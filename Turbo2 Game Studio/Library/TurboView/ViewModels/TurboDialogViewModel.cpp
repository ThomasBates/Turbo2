
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
    _okViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(okButton, "TextButton"));

    if (_showCancel)
    {
        auto cancelButton = std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(TurboGameControlType::Action, 2, 0, 0));
        _cancelViewModel = std::shared_ptr<ITurboControlViewModel>(new TurboControlViewModel(cancelButton, "TextButton"));
    }
}

void TurboDialogViewModel::Update()
{
    _okAction = !_lastOKButtonActive && _okViewModel->IsActive();
    _lastOKButtonActive = _okViewModel->IsActive();

    if (_showCancel)
    {
        _cancelAction = !_lastCancelButtonActive && _cancelViewModel->IsActive();
        _lastCancelButtonActive = _cancelViewModel->IsActive();
    }

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
