
#include <pch.h>

#include <MazePreviewMainView.h>
#include <MazePreviewHUD1View.h>
#include <MazePreviewHUD2View.h>
#include <MazePreviewMotionControlView.h>

#include <TurboControlView.h>
#include <TurboSceneSprite.h>
#include <TurboSceneTexture.h>
#include <TurboSceneView.h>
#include <TurboSceneSound.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewMainView::MazePreviewMainView(
        std::shared_ptr<ITurboDebug> debug,
        std::string name,
        const std::shared_ptr<ITurboViewRendererAccess>& rendererAccess,
        std::shared_ptr<MazePreviewMainViewModel> viewModel) :
        TurboGroupView(std::move(name), rendererAccess),
        _debug(std::move(debug)),
        _viewModel(std::move(viewModel))
{
    _sceneView = std::shared_ptr<ITurboView>(new TurboSceneView("Main Scene View", rendererAccess, _viewModel->MainSceneViewModel()));
    _hud1View = std::shared_ptr<ITurboView>(new MazePreviewHUD1View(_debug, "HUD1 View", rendererAccess, _viewModel->HUD1ViewModel()));
    _hud2View = std::shared_ptr<ITurboView>(new MazePreviewHUD2View(_debug, "HUD2 View", rendererAccess, _viewModel->HUD2ViewModel()));

    _mainControlView = std::shared_ptr<ITurboView>(new TurboControlView("Main Control View", rendererAccess, _viewModel->MainControlViewModel()));
    _motionControlView = std::shared_ptr<ITurboView>(new MazePreviewMotionControlView("Motion Control View", rendererAccess, _viewModel->MotionControlViewModel()));
    _strafeControlView = std::shared_ptr<ITurboView>(new TurboControlView("Strafe Control View", rendererAccess, _viewModel->GetStrafeControlViewModel(), "DPadControl"));
    _directionControlView = std::shared_ptr<ITurboView>(new TurboControlView("Direction Control View", rendererAccess, _viewModel->DirectionControlViewModel(), "DPadControl"));

    AddView(_sceneView);
    AddView(_hud1View);
    AddView(_hud2View);

    AddView(_motionControlView);
    AddView(_strafeControlView);
    AddView(_directionControlView);
    AddView(_mainControlView);

    auto texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("Watermark"));
    _watermark = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));

//    _background = std::shared_ptr<ITurboSceneSound>(new TurboSceneSound("Entrance"));
//    _background->PlaySound(true);
//    _background->Volume(0.10F);
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewMainView::Load()
{
    TurboGroupView::Load();

    LoadSceneSprite(_watermark);
    //LoadSceneBackground(_background);
}

void MazePreviewMainView::Render()
{
    TurboGroupView::Render();

    RenderSceneSprite(_watermark);
    //RenderSceneBackground(_background);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewMainView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _debug->Send(debugDebug, debugView) << "MazePreviewMainView::UpdateLayout: position = " << position << ", size = " << size << "\n";

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

    _debug->Send(debugDebug, debugView) << "MazePreviewMainView::UpdateLayout: s = " << s << ", x1 = " << x1 << ", x2 = " << x2 << ", y1 = " << y1 << ", y2 = " << y2 << "\n";

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
    _watermark->Rectangle(TurboRectangle(
            x2 - s/4,
            y2 - s/4,
            x2,
            y2));

    // bottom left
    _motionControlView->Position(TurboVector2D(0, height - s));
    _motionControlView->Size(TurboVector2D(s, s));

    // bottom right
    _directionControlView->Position(TurboVector2D(width - s, height - s));
    _directionControlView->Size(TurboVector2D(s, s));

    // extra
    if (width < height)
    {
        _strafeControlView->Position(TurboVector2D(0,height - 2 * s));
    }
    else
    {
        _strafeControlView->Position(TurboVector2D(s,height - s));
    }
    _strafeControlView->Size(TurboVector2D(s, s));
}
