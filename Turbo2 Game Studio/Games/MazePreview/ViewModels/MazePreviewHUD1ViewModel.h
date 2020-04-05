
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

private:
    std::shared_ptr<ITurboGame> _game;
};
