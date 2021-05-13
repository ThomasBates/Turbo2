
#pragma once

#include <pch.h>

#include <TurboSceneViewModel.h>
#include <MazePreviewHUD1ViewModel.h>
#include <MazePreviewHUD2ViewModel.h>
#include <MazePreviewMainControlViewModel.h>
#include <MazePreviewMotionControlViewModel.h>
#include <MazePreviewDirectionControlViewModel.h>

using namespace Turbo::Game;

class MazePreviewInfoViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewInfoViewModel(std::shared_ptr<ITurboGame> game);
    virtual ~MazePreviewInfoViewModel() {}

    //  Public Methods -------------------------------------------------------------------------------------------------
    void Update();

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<TurboSceneViewModel> MainSceneViewModel() { return _sceneViewModel; }
    std::shared_ptr<ITurboControlViewModel> CloseViewModel() { return _closeViewModel; }
    std::string InfoText();

    bool IsVisible() { return _isVisible; }
    void IsVisible(bool isVisible) { _isVisible = isVisible; }
    bool CloseAction() { return _closeAction; }

private:
    std::shared_ptr<TurboSceneViewModel> _sceneViewModel;
    std::shared_ptr<ITurboControlViewModel> _closeViewModel;
    std::string _infoText = "This is a long text string that will be replaced with "
                            "instructions for how to play MazePreview.";

    bool _isVisible;
    bool _lastCloseButtonActive;
    bool _closeAction;
};
