
#pragma once

#include <pch.h>

#include <ITurboGame.h>
#include <ITurboControlViewModel.h>
#include <TurboSceneViewModel.h>

using namespace Turbo::Game;
using namespace Turbo::View;

class TurboDialogViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    TurboDialogViewModel(std::shared_ptr<ITurboGame> game, bool showCancel);
    virtual ~TurboDialogViewModel() {}

    //  Public Methods -------------------------------------------------------------------------------------------------
    virtual void Update();

    //  Public Properties ----------------------------------------------------------------------------------------------
    //std::shared_ptr<TurboSceneViewModel> MainSceneViewModel() { return _sceneViewModel; }
    std::shared_ptr<ITurboControlViewModel> OKViewModel() { return _okViewModel; }
    std::shared_ptr<ITurboControlViewModel> CancelViewModel() { return _cancelViewModel; }

    bool ShowCancel() { return _showCancel; }

    std::string DialogText() { return _dialogText; }
    void DialogText(std::string dialogText) { _dialogText = dialogText; }

    std::string CaptionText() { return _captionText; }
    void CaptionText(std::string captionText) { _captionText = captionText; }

    bool IsVisible() { return _isVisible; }
    void IsVisible(bool isVisible);

    bool OKAction() { return _okAction; }
    bool CancelAction() { return _cancelAction; }

protected:
    virtual void DoCustomLoadData() {}
    virtual void DoCustomSaveData() {}
    virtual void DoCustomUndoChanges() {}

private:
    bool _showCancel;

    //std::shared_ptr<TurboSceneViewModel> _sceneViewModel;
    std::shared_ptr<ITurboControlViewModel> _okViewModel;
    std::shared_ptr<ITurboControlViewModel> _cancelViewModel;

    std::string _captionText;
    std::string _dialogText;

    bool _isVisible;
    bool _okAction;
    bool _cancelAction;
};
