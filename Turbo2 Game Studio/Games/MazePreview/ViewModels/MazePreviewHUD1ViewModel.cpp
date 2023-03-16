
#include <pch.h>

#include <MazePreviewHUD1ViewModel.h>


MazePreviewHUD1ViewModel::MazePreviewHUD1ViewModel(std::shared_ptr<ITurboGame> game) :
    _game(game)
{
}

std::string MazePreviewHUD1ViewModel::RoundText()
{
    if (_checkRound)
    {
        auto state = _game->GameState();
        auto round = state->LoadString("Game.CurrentRound", "1");
        _roundText = " Round " + round;
    }
    return _roundText;
}

std::string MazePreviewHUD1ViewModel::TitleText()
{
    auto title = _game->Title();
    if (_title != title)
    {
        _title = title;
        _titleText = " " + title;
        _checkRound = true;
    }
    return _titleText;
}

std::string MazePreviewHUD1ViewModel::KeysText()
{
    if (_title == "Mezzanine")
        return "";
    return " #/# keys";
}

std::string MazePreviewHUD1ViewModel::HazardText()
{
    if (_title == "Mezzanine")
        return "";
    return " # hazards";
}

