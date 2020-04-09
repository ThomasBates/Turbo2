
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

private:
    std::shared_ptr<ITurboGame> _game;
    bool _checkRound = true;
    std::string _roundText;
    std::string _title;
    std::string _titleText;
};
