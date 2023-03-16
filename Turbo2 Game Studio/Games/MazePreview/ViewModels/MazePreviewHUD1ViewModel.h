
#pragma once

#include <pch.h>

#include <ITurboGame.h>

using namespace Turbo::Game;

class MazePreviewHUD1ViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewHUD1ViewModel(std::shared_ptr<ITurboGame> game);
    virtual ~MazePreviewHUD1ViewModel() {}

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::string TitleText();
    std::string RoundText();
    std::string KeysText();
    std::string HazardText();

private:
    std::shared_ptr<ITurboGame> _game;
    bool _checkRound = true;
    std::string _roundText;
    std::string _title;
    std::string _titleText;
    std::string _keysText;
    std::string _hazardText;
};
