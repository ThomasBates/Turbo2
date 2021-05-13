
#pragma once

#include <pch.h>

#include <MazePreviewMainViewModel.h>
#include <MazePreviewMenuViewModel.h>
#include <MazePreviewInfoViewModel.h>

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
    std::shared_ptr<MazePreviewInfoViewModel> InfoViewModel() { return _infoViewModel; }

    bool MainViewVisible() { return _mainViewModel->IsVisible(); }
    bool MenuViewVisible() { return _menuViewModel->IsVisible(); }
    bool InfoViewVisible() { return _infoViewModel->IsVisible(); }

private:
    std::shared_ptr<MazePreviewMainViewModel> _mainViewModel;
    std::shared_ptr<MazePreviewMenuViewModel> _menuViewModel;
    std::shared_ptr<MazePreviewInfoViewModel> _infoViewModel;
};
