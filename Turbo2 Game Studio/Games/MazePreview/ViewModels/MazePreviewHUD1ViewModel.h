
#pragma once

#include <pch.h>

#include <ITurboGame.h>
#include <MazePreviewGameState.h>

using namespace Turbo::Game;

class MazePreviewHUD1ViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewHUD1ViewModel(std::shared_ptr<ITurboGame> game, std::shared_ptr<MazePreviewGameState> gameState);
    virtual ~MazePreviewHUD1ViewModel() {}

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::string RoundText();
    std::string TitleText();
    std::string KeysText();
    std::string HazardText();
    bool IsUnlocked() { return _isUnlocked; }

    //  Event Handlers -------------------------------------------------------------------------------------------------
    void GameStateOnValueChanged(void *sender, std::shared_ptr<TurboConfigValueChangedEventArgs> args);

private:
    std::shared_ptr<ITurboGame> _game;
    std::shared_ptr<MazePreviewGameState> _gameState;

    std::shared_ptr<ITurboConfigTypedItem<int>> _currentRound;
    std::shared_ptr<ITurboConfigTypedItem<int>> _requiredKeyCount;
    std::shared_ptr<ITurboConfigTypedItem<int>> _foundKeyCount;
    std::shared_ptr<ITurboConfigTypedItem<int>> _hazardCount;

    std::string _title;
    bool _isUnlocked = false;

    void UpdateFields();
};
