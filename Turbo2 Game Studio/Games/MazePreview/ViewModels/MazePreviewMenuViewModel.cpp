
#include <pch.h>

#include <MazePreviewMenuViewModel.h>
#include <TurboSceneNavigationPadControl.h>
#include <TurboSceneNavigationButtonControl.h>
#include <TurboControlViewModel.h>
#include <TurboSceneNavigationButtonControl.h>

MazePreviewMenuViewModel::MazePreviewMenuViewModel(std::shared_ptr<ITurboGame> game) :
    TurboDialogViewModel(game, true),
    _game(game)
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

    _gameState = _game->GameState();

    _controlsReversed = _gameState->LoadBoolean("User.InvertedMouse", false);
    _soundEffectsOn = _gameState->LoadBoolean("User.SoundEffectsOn", true);
    _selectedRound = _gameState->LoadInteger("Game.CurrentRound", 1);
    _selectedLevel = _gameState->LoadInteger("Game.CurrentLevel", 1);

    _reverseControlViewModel->SetValue(_controlsReversed);
    _soundEffectsViewModel->SetValue(_soundEffectsOn);
    _selectRoundViewModel->SetValue(_selectedRound);
    _selectLevelViewModel->SetValue(_selectedLevel);
    _resetProgressViewModel->SetValue(false);

    _originalControlsReversed = _controlsReversed;
    _originalSoundEffectsOn = _soundEffectsOn;
    _originalSelectedRound = _selectedRound;
    _originalSelectedLevel = _selectedLevel;

    int unlockedRound = _gameState->LoadInteger("Game.UnlockedRound", 1);
    _selectRoundViewModel->SetMaxValue(unlockedRound);
}

void MazePreviewMenuViewModel::DoCustomSaveData()
{
    TurboDialogViewModel::DoCustomSaveData();

    auto stateChanged = false;
    if (_originalControlsReversed != _controlsReversed)
    {
        _gameState->SaveBoolean("User.InvertedMouse", _controlsReversed);
        stateChanged = true;
    }
    if (_originalSoundEffectsOn != _soundEffectsOn)
    {
        _gameState->SaveBoolean("User.SoundEffectsOn", _soundEffectsOn);
        stateChanged = true;
    }
    if (_originalSelectedRound != _selectedRound)
    {
        _gameState->SaveInteger("Game.CurrentRound", _selectedRound);
        stateChanged = true;
    }
    if (_originalSelectedLevel != _selectedLevel)
    {
        _gameState->SaveInteger("Game.CurrentLevel", _selectedLevel);
        stateChanged = true;
    }
    if (_resetProgress)
    {
        _gameState->SaveInteger("Game.UnlockedRound", 1);
        _gameState->SaveInteger("Game.UnlockedLevel", 1);
        _gameState->SaveInteger("Game.CurrentRound", 1);
        _gameState->SaveInteger("Game.CurrentLevel", 1);
        stateChanged = true;
    }

    if (stateChanged)
        _game->GameState(_gameState);
}

void MazePreviewMenuViewModel::DoCustomUndoChanges()
{
    TurboDialogViewModel::DoCustomUndoChanges();

    //_reverseControlViewModel->SetValue(_originalControlsReversed);
}
