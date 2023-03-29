
#include <pch.h>

#include <MazePreviewHUD1ViewModel.h>
#include <TurboEventHandler.h>

#include <utility>


MazePreviewHUD1ViewModel::MazePreviewHUD1ViewModel(std::shared_ptr<ITurboGame> game, std::shared_ptr<MazePreviewGameState> gameState) :
    _game(std::move(game)),
    _gameState(std::move(gameState))
{
    _gameState->Game()->OnValueChanged()->Subscribe(std::shared_ptr<ITurboEventHandler<std::shared_ptr<TurboConfigValueChangedEventArgs>>>(
            new TurboEventHandler<MazePreviewHUD1ViewModel, std::shared_ptr<TurboConfigValueChangedEventArgs>>(this, &MazePreviewHUD1ViewModel::GameStateOnValueChanged)));

    _currentRound = _gameState->Game()->CurrentRound();
    _requiredKeyCount = _gameState->Maze()->RequiredKeyCount();
    _foundKeyCount = _gameState->Maze()->FoundKeyCount();
    _hazardCount = _gameState->Maze()->HazardCount();

    UpdateFields();
}

std::string MazePreviewHUD1ViewModel::RoundText()
{
    return "Round " + _currentRound->GetAsString();
}

std::string MazePreviewHUD1ViewModel::TitleText()
{
    _title = _game->Title();
    return _title;
}

std::string MazePreviewHUD1ViewModel::KeysText()
{
    if (_title == "Mezzanine")
    {
        return "";
    }

    int required = _requiredKeyCount->GetValue();
    if (required < 1)
    {
        return "";
    }

    int found = _foundKeyCount->GetValue();
    _isUnlocked = (found >= required);

    return "Keys Found: " + _foundKeyCount->GetAsString() + " / " + _requiredKeyCount->GetAsString();
}

std::string MazePreviewHUD1ViewModel::HazardText()
{
    if (_title == "Mezzanine")
    {
        return "";
    }

    if (_hazardCount->GetValue() < 1)
    {
        return "";
    }

    return "Hazards: " + _hazardCount->GetAsString();
}

void MazePreviewHUD1ViewModel::GameStateOnValueChanged(void *sender, std::shared_ptr<TurboConfigValueChangedEventArgs> args)
{
    UpdateFields();
}

void MazePreviewHUD1ViewModel::UpdateFields()
{
}

