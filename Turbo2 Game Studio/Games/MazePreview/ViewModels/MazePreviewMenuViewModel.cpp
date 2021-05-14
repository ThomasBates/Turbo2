
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
}

void MazePreviewMenuViewModel::Update()
{
    _controlsReversed = _reverseControlViewModel->ToggleOn();
    _soundEffectsOn = _soundEffectsViewModel->ToggleOn();

    TurboDialogViewModel::Update();
}

void MazePreviewMenuViewModel::DoCustomLoadData()
{
    TurboDialogViewModel::DoCustomLoadData();

    _gameState = _game->GameState();

    _controlsReversed = _gameState->LoadBoolean("User.InvertedMouse", false);
    _soundEffectsOn = _gameState->LoadBoolean("User.SoundEffectsOn", true);

    _reverseControlViewModel->ToggleOn(_controlsReversed);
    _soundEffectsViewModel->ToggleOn(_soundEffectsOn);

    _originalControlsReversed = _controlsReversed;
    _originalSoundEffectsOn = _soundEffectsOn;
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

    if (stateChanged)
        _game->GameState(_gameState);
}

void MazePreviewMenuViewModel::DoCustomUndoChanges()
{
    TurboDialogViewModel::DoCustomUndoChanges();

    //_reverseControlViewModel->ToggleOn(_originalControlsReversed);
}
