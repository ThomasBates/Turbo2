
#include <pch.h>

#include <MazePreviewRootView.h>
#include <MazePreviewMainView.h>
#include <MazePreviewMenuView.h>
#include <MazePreviewInfoView.h>

#include <TurboSceneSprite.h>
#include <TurboSceneTexture.h>
#include <TurboSceneView.h>
#include <TurboSceneSound.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewRootView::MazePreviewRootView(
        std::shared_ptr<ITurboDebug> debug,
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewRootViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _debug(debug),
        _viewModel(viewModel)
{
    _mainView = std::shared_ptr<ITurboView>(new MazePreviewMainView(debug, "Main View", rendererAccess, _viewModel->MainViewModel()));
    _menuView = std::shared_ptr<ITurboView>(new MazePreviewMenuView(debug, "Menu View", rendererAccess, _viewModel->MenuViewModel()));
    _infoView = std::shared_ptr<ITurboView>(new MazePreviewInfoView(debug, "Info View", rendererAccess, _viewModel->InfoViewModel()));

//    _mainView->IsVisible(true);
//    _menuView->IsVisible(false);
//    _infoView->IsVisible(false);

    InternalAddView(_mainView);
    InternalAddView(_menuView);
    InternalAddView(_infoView);
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewRootView::Load()
{
    _viewModel->Update();

    _mainView->IsVisible(_viewModel->MainViewVisible());
    _menuView->IsVisible(_viewModel->MenuViewVisible());
    _infoView->IsVisible(_viewModel->InfoViewVisible());

    TurboGroupView::Load();
}

void MazePreviewRootView::Render()
{
    TurboGroupView::Render();
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewRootView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _debug->Send(debugDebug, debugView) << "MazePreviewRootView::UpdateLayout: position = " << position << ", size = " << size << "\n";

    //  main view (whole screen)
    _mainView->Position(position);
    _mainView->Size(size);

    //  options view (whole screen)
    _menuView->Position(position);
    _menuView->Size(size);

    //  instructions view (whole screen)
    _infoView->Position(position);
    _infoView->Size(size);
}
