
#pragma once

#include <pch.h>

#include <ITurboGame.h>
#include <ITurboControlViewModel.h>

using namespace Turbo::Game;
using namespace Turbo::View;

class MazePreviewHUD2ViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewHUD2ViewModel(std::shared_ptr<ITurboGame> game);
    virtual ~MazePreviewHUD2ViewModel() {}

    //  Public Methods -------------------------------------------------------------------------------------------------
    void Update();

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<ITurboControlViewModel> MenuViewModel() { return _openMenuViewModel; }
    std::shared_ptr<ITurboControlViewModel> InfoViewModel() { return _openInfoViewModel; }

    bool OpenMenuAction() { return _openMenuAction; }
    bool OpenInfoAction() { return _openInfoAction; }

    std::string SignageText() { return _signageText; }

private:
    std::shared_ptr<ITurboGame> _game;

    std::shared_ptr<ITurboControlViewModel> _openMenuViewModel;
    std::shared_ptr<ITurboControlViewModel> _openInfoViewModel;

    bool _openMenuAction;
    bool _openInfoAction;

    std::string _signageText;
};
