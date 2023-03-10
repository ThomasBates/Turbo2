
#pragma once

#include <pch.h>

#include <MazePreviewMainViewModel.h>
#include <MazePreviewMenuViewModel.h>
#include <ViewModels/TurboDialogViewModel.h>

using namespace Turbo::Game;

class MazePreviewRootViewModel
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewRootViewModel(const std::shared_ptr<ITurboGame>& game);
    virtual ~MazePreviewRootViewModel() {}

    //  Public Methods -------------------------------------------------------------------------------------------------
    void Update();

    //  Public Properties ----------------------------------------------------------------------------------------------
    std::shared_ptr<MazePreviewMainViewModel> MainViewModel() { return _mainViewModel; }
    std::shared_ptr<MazePreviewMenuViewModel> MenuViewModel() { return _menuViewModel; }
    std::shared_ptr<TurboDialogViewModel> InfoViewModel() { return _infoViewModel; }

    bool MainViewVisible() { return _mainViewModel->IsVisible(); }
    bool MenuViewVisible() { return _menuViewModel->IsVisible(); }
    bool InfoViewVisible() { return _infoViewModel->IsVisible(); }

private:
    std::shared_ptr<ITurboGame> _game;
    std::shared_ptr<MazePreviewMainViewModel> _mainViewModel;
    std::shared_ptr<MazePreviewMenuViewModel> _menuViewModel;
    std::shared_ptr<TurboDialogViewModel> _infoViewModel;

    std::string _infoText = "This is a long text string that will be replaced with "
                            "instructions for how to play MazePreview.";
};
