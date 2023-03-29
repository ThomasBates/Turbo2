
#include <pch.h>

#include <MazePreviewMenuViewModel.h>
#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>
#include <TurboControlViewModel.h>
#include <TurboSceneNavigationButtonControl.h>

MazePreviewMenuViewModel::MazePreviewMenuViewModel(
    const std::shared_ptr<ITurboGame>& game,
    std::shared_ptr<MazePreviewGameState> gameState) :
        TurboDialogViewModel(game, true),
        _game(game),
        _gameState(std::move(gameState))
{
    TurboDialogViewModel::CaptionText("Options Menu");

    _reverseControlViewModel = std::shared_ptr<TurboToggleControlViewModel>(new TurboToggleControlViewModel());
    _soundEffectsViewModel = std::shared_ptr<TurboToggleControlViewModel>(new TurboToggleControlViewModel());
    _selectRoundViewModel = std::shared_ptr<TurboSpinnerControlViewModel>(new TurboSpinnerControlViewModel());
    _selectLevelViewModel = std::shared_ptr<TurboSpinnerControlViewModel>(new TurboSpinnerControlViewModel());
    _resetProgressViewModel = std::shared_ptr<TurboToggleControlViewModel>(new TurboToggleControlViewModel());

    _selectRoundViewModel->SetMinValue(1);
    _selectRoundViewModel->SetMaxValue(1);
    _selectLevelViewModel->SetMinValue(1);
    _selectLevelViewModel->SetMaxValue(4);
}

void MazePreviewMenuViewModel::Update()
{
    _controlsReversed = _reverseControlViewModel->GetValue();
    _soundEffectsOn = _soundEffectsViewModel->GetValue();
    _selectedRound = _selectRoundViewModel->GetValue();
    _selectedLevel = _selectLevelViewModel->GetValue();
    _resetProgress = _resetProgressViewModel->GetValue();

    TurboDialogViewModel::Update();
}

void MazePreviewMenuViewModel::DoCustomLoadData()
{
    TurboDialogViewModel::DoCustomLoadData();

    _gameState->BeginBatch();

    _controlsReversed = _gameState->User()->InvertedMouse()->GetValue();
    _soundEffectsOn = _gameState->User()->SoundEffectsOn()->GetValue();
    _selectedRound = _gameState->Game()->CurrentRound()->GetValue();
    _selectedLevel = _gameState->Game()->CurrentLevel()->GetValue();

    _reverseControlViewModel->SetValue(_controlsReversed);
    _soundEffectsViewModel->SetValue(_soundEffectsOn);
    _selectRoundViewModel->SetValue(_selectedRound);
    _selectLevelViewModel->SetValue(_selectedLevel);
    _resetProgressViewModel->SetValue(false);

    int unlockedRound = _gameState->Game()->UnlockedRound()->GetValue();
    _selectRoundViewModel->SetMaxValue(unlockedRound);

    _gameState->EndBatch();
}

void MazePreviewMenuViewModel::DoCustomSaveData()
{
    TurboDialogViewModel::DoCustomSaveData();

    _gameState->BeginBatch();

    _gameState->User()->InvertedMouse()->SetValue(_controlsReversed);
    _gameState->User()->SoundEffectsOn()->SetValue(_soundEffectsOn);
    _gameState->Game()->CurrentRound()->SetValue(_selectedRound);
    _gameState->Game()->CurrentLevel()->SetValue(_selectedLevel);

    if (_resetProgress)
    {
        _gameState->Game()->UnlockedRound()->SetValue(1);
        _gameState->Game()->UnlockedLevel()->SetValue(1);
        _gameState->Game()->CurrentRound()->SetValue(1);
        _gameState->Game()->CurrentLevel()->SetValue(1);
    }

    _gameState->EndBatch();
}

void MazePreviewMenuViewModel::DoCustomUndoChanges()
{
    TurboDialogViewModel::DoCustomUndoChanges();

    //_reverseControlViewModel->SetValue(_originalControlsReversed);
}
