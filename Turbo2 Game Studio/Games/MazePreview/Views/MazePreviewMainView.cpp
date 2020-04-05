
#include <pch.h>

#include <MazePreviewMainView.h>
#include <MazePreviewHUD1View.h>
#include <MazePreviewHUD2View.h>
#include <MazePreviewMainControlView.h>
#include <MazePreviewMotionControlView.h>
#include <MazePreviewDirectionControlView.h>

#include <TurboSceneSprite.h>
#include <TurboSceneTexture.h>
#include <TurboSceneView.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewMainView::MazePreviewMainView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewMainViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _viewModel(viewModel)
{
    _sceneView = std::shared_ptr<ITurboView>(new TurboSceneView("Main Scene View", rendererAccess, _viewModel->MainSceneViewModel()));
    _hud1View = std::shared_ptr<ITurboView>(new MazePreviewHUD1View("HUD1 View", rendererAccess, _viewModel->HUD1ViewModel()));
    _hud2View = std::shared_ptr<ITurboView>(new MazePreviewHUD2View("HUD2 View", rendererAccess, _viewModel->HUD2ViewModel()));

    _mainControlView = std::shared_ptr<ITurboView>(new MazePreviewMainControlView("Main Control View", rendererAccess, _viewModel->MainControlViewModel()));
    _motionControlView = std::shared_ptr<ITurboView>(new MazePreviewMotionControlView("Motion Control View", rendererAccess, _viewModel->MotionControlViewModel()));
    _directionControlView = std::shared_ptr<ITurboView>(new MazePreviewDirectionControlView("Direction Control View", rendererAccess, _viewModel->DirectionControlViewModel()));

    AddView(_sceneView);
    AddView(_hud1View);
    AddView(_hud2View);

    AddView(_mainControlView);
    AddView(_motionControlView);
    AddView(_directionControlView);

    auto texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("BackwardButton"));
    _watermark = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewMainView::Load()
{
    TurboGroupView::Load();

    LoadSceneSprite(_watermark);
}

void MazePreviewMainView::Render()
{
    TurboGroupView::Render();

    RenderSceneSprite(_watermark);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewMainView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    float width = size.X;
    float height = size.Y;

    float s;
    float x1;
    float x2;
    float y1;
    float y2;

    if (width < height)
    {
        s = width / 2;

        // center
        x1 = 0;
        x2 = width;
        y1 = s;
        y2 = height - s;
    }
    else
    {
        s = height / 2;

        // center
        x1 = s;
        x2 = width - s;
        y1 = 0;
        y2 = height;
    }

    //  main scene (whole screen)
    _sceneView->Position(TurboVector2D(0, 0));
    _sceneView->Size(TurboVector2D(width, height));

    //  HUD 1
    _hud1View->Position(TurboVector2D(0, 0));
    _hud1View->Size(TurboVector2D(s, s));

    //  HUD 2
    _hud2View->Position(TurboVector2D(width - s, 0));
    _hud2View->Size(TurboVector2D(s, s));

    // center control
    _mainControlView->Position(TurboVector2D(x1, y1));
    _mainControlView->Size(TurboVector2D(x2 - x1, y2 - y1));

    // watermark
    _watermark->Left(x2 - s/4);
    _watermark->Top(y2 - s/4);
    _watermark->Right(x2);
    _watermark->Bottom(y2);

    // bottom left
    _motionControlView->Position(TurboVector2D(0, height - s));
    _motionControlView->Size(TurboVector2D(s, s));

    // bottom right
    _directionControlView->Position(TurboVector2D(width - s, height - s));
    _directionControlView->Size(TurboVector2D(s, s));
}
