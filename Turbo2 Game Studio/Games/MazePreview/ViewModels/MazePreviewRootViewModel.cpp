
#include <pch.h>

#include <MazePreviewRootViewModel.h>

MazePreviewRootViewModel::MazePreviewRootViewModel(const std::shared_ptr<ITurboGame>& game) : _game(game)
{
    _mainViewModel = std::shared_ptr<MazePreviewMainViewModel>(new MazePreviewMainViewModel(game));
    _menuViewModel = std::shared_ptr<MazePreviewMenuViewModel>(new MazePreviewMenuViewModel(game));
    _infoViewModel = std::shared_ptr<TurboDialogViewModel>(new TurboDialogViewModel(game, false));

    _infoViewModel->CaptionText("Introduction");
    _infoViewModel->DialogText(_infoText);

    _mainViewModel->IsVisible(true);
    _menuViewModel->IsVisible(false);
    _infoViewModel->IsVisible(false);
}

void MazePreviewRootViewModel::Update()
{
    if (_mainViewModel->IsVisible())
    {
        _mainViewModel->Update();

        if (_mainViewModel->OpenMenuAction())
        {
            _mainViewModel->IsVisible(false);
            _menuViewModel->IsVisible(true);
        }

        if (_mainViewModel->OpenInfoAction())
        {
            _mainViewModel->IsVisible(false);
            _infoViewModel->IsVisible(true);
        }
    }

    if (_menuViewModel->IsVisible())
    {
        _menuViewModel->Update();

        if (_menuViewModel->OKAction() ||
            _menuViewModel->CancelAction())
        {
            _menuViewModel->IsVisible(false);
            _mainViewModel->IsVisible(true);
        }
    }

    if (_infoViewModel->IsVisible())
    {
        _infoViewModel->Update();

        if (_infoViewModel->OKAction())
        {
            _infoViewModel->IsVisible(false);
            _mainViewModel->IsVisible(true);
        }
    }
}
