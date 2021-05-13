
#include <pch.h>

#include <MazePreviewMenuView.h>
#include <MazePreviewHUD1View.h>
#include <MazePreviewHUD2View.h>
#include <MazePreviewMainControlView.h>
#include <MazePreviewMotionControlView.h>
#include <MazePreviewDirectionControlView.h>

#include <TurboSceneSprite.h>
#include <TurboSceneTexture.h>
#include <TurboSceneView.h>
#include <TurboSceneSound.h>
#include <Views/TurboControlView.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewMenuView::MazePreviewMenuView(
        std::shared_ptr<ITurboDebug> debug,
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewMenuViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _debug(debug),
        _viewModel(viewModel)
{
    _sceneView = std::shared_ptr<ITurboView>(new TurboSceneView("Main Scene View", rendererAccess, _viewModel->MainSceneViewModel()));

    _closeControlView = std::shared_ptr<ITurboView>(new TurboControlView("Menu Button View", rendererAccess, _viewModel->CloseViewModel()));

    InternalAddView(_sceneView);
    InternalAddView(_closeControlView);
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewMenuView::Load()
{
    TurboGroupView::Load();

    //LoadSceneSprite(_watermark);
    //LoadSceneBackground(_background);
}

void MazePreviewMenuView::Render()
{
    TurboGroupView::Render();

    //RenderSceneSprite(_watermark);
    //RenderSceneBackground(_background);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewMenuView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _debug->Send(debugDebug, debugView) << "MazePreviewMenuView::UpdateLayout: position = " << position << ", size = " << size << "\n";

    //  main scene (whole screen)
    _sceneView->Position(position);
    _sceneView->Size(size);

    float width = (size.X < size.Y) ? size.X / 8 : size.Y / 8;

    _closeControlView->Position(TurboVector2D(position.X + size.X - width, position.Y));
    _closeControlView->Size(TurboVector2D(width, width));
}
