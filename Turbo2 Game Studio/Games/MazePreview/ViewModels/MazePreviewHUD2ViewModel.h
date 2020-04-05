
#pragma once

#include <pch.h>

#include <ITurboGame.h>

using namespace Turbo::Game;

class MazePreviewHUD2ViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewHUD2ViewModel(std::shared_ptr<ITurboGame> game);
    virtual ~MazePreviewHUD2ViewModel() {}

    //  Public Properties ----------------------------------------------------------------------------------------------

private:
    std::shared_ptr<ITurboGame> _game;
};
