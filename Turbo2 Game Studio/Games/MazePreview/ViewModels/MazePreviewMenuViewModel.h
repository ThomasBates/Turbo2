
#pragma once

#include <pch.h>

#include <TurboSceneViewModel.h>
#include <TurboSpinnerControlViewModel.h>
#include <TurboToggleControlViewModel.h>
#include <MazePreviewGameState.h>
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
    MazePreviewMenuViewModel(const std::shared_ptr<ITurboGame>& game, std::shared_ptr<MazePreviewGameState> gameState);
    virtual ~MazePreviewMenuViewModel() {}

    //  Public Methods -------------------------------------------------------------------------------------------------
    virtual void Update();

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<TurboToggleControlViewModel> ReverseControlViewModel() { return _reverseControlViewModel; }
    std::shared_ptr<TurboToggleControlViewModel> SoundEffectsViewModel() { return _soundEffectsViewModel; }
    std::shared_ptr<TurboSpinnerControlViewModel> SelectRoundViewModel() { return _selectRoundViewModel; }
    std::shared_ptr<TurboSpinnerControlViewModel> SelectLevelViewModel() { return _selectLevelViewModel; }
    std::shared_ptr<TurboToggleControlViewModel> ResetProgressViewModel() { return _resetProgressViewModel; }

protected:
    virtual void DoCustomLoadData();
    virtual void DoCustomSaveData();
    virtual void DoCustomUndoChanges();

private:
    std::shared_ptr<ITurboGame> _game;
    std::shared_ptr<MazePreviewGameState> _gameState;

    std::shared_ptr<TurboToggleControlViewModel> _reverseControlViewModel;
    std::shared_ptr<TurboToggleControlViewModel> _soundEffectsViewModel;
    std::shared_ptr<TurboSpinnerControlViewModel> _selectRoundViewModel;
    std::shared_ptr<TurboSpinnerControlViewModel> _selectLevelViewModel;
    std::shared_ptr<TurboToggleControlViewModel> _resetProgressViewModel;

    bool _controlsReversed = false;
    bool _soundEffectsOn = true;
    int _selectedRound = 1;
    int _selectedLevel = 1;
    bool _resetProgress = false;
};
