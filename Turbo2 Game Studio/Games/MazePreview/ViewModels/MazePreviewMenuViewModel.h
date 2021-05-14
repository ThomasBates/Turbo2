
#pragma once

#include <pch.h>

#include <TurboSceneViewModel.h>
#include <TurboToggleControlViewModel.h>
#include <MazePreviewHUD1ViewModel.h>
#include <MazePreviewHUD2ViewModel.h>
#include <MazePreviewMainControlViewModel.h>
#include <MazePreviewMotionControlViewModel.h>
#include <MazePreviewDirectionControlViewModel.h>
#include <ViewModels/TurboDialogViewModel.h>

using namespace Turbo::Game;

class MazePreviewMenuViewModel : public TurboDialogViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewMenuViewModel(std::shared_ptr<ITurboGame> game);
    virtual ~MazePreviewMenuViewModel() {}

    //  Public Methods -------------------------------------------------------------------------------------------------
    virtual void Update();

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<TurboToggleControlViewModel> ReverseControlViewModel() { return _reverseControlViewModel; }
    std::shared_ptr<TurboToggleControlViewModel> SoundEffectsViewModel() { return _soundEffectsViewModel; }

protected:
    virtual void DoCustomLoadData();
    virtual void DoCustomSaveData();
    virtual void DoCustomUndoChanges();

private:
    std::shared_ptr<ITurboGame> _game;
    std::shared_ptr<ITurboGameState> _gameState;

    std::shared_ptr<TurboToggleControlViewModel> _reverseControlViewModel;
    std::shared_ptr<TurboToggleControlViewModel> _soundEffectsViewModel;

    bool _originalControlsReversed;
    bool _originalSoundEffectsOn;
    bool _controlsReversed;
    bool _soundEffectsOn;
};
