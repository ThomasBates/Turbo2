
#include <pch.h>

#include <MazePreviewInfoView.h>
#include <MazePreviewHUD1View.h>
#include <MazePreviewHUD2View.h>
#include <MazePreviewMainControlView.h>
#include <MazePreviewMotionControlView.h>
#include <MazePreviewDirectionControlView.h>

#include <TurboSceneView.h>
#include <TurboSceneText.h>
#include <Views/TurboControlView.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewInfoView::MazePreviewInfoView(
        std::shared_ptr<ITurboDebug> debug,
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewInfoViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _debug(debug),
        _viewModel(viewModel)
{
    _sceneView = std::shared_ptr<ITurboView>(new TurboSceneView("Main Scene View", rendererAccess, _viewModel->MainSceneViewModel()));
    _closeControlView = std::shared_ptr<ITurboView>(new TurboControlView("Menu Button View", rendererAccess, _viewModel->CloseViewModel()));
    _infoText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(1.0F, 0.0F, 0.0F), _viewModel->InfoText()));
    _infoText->UseRectangle(true);

    InternalAddView(_sceneView);
    InternalAddView(_closeControlView);
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewInfoView::Load()
{
    TurboGroupView::Load();

    _infoText->Text(_viewModel->InfoText());

    LoadSceneText(_infoText);
}

void MazePreviewInfoView::Render()
{
    TurboGroupView::Render();

    RenderSceneText(_infoText);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewInfoView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _debug->Send(debugDebug, debugView) << "MazePreviewInfoView::UpdateLayout: position = " << position << ", size = " << size << "\n";

    //  main scene (whole screen)
    _sceneView->Position(position);
    _sceneView->Size(size);

    float width = (size.X < size.Y) ? size.X / 8 : size.Y / 8;

    _closeControlView->Position(TurboVector2D(position.X + size.X - width, position.Y));
    _closeControlView->Size(TurboVector2D(width, width));

    _infoText->Rectangle(TurboRectangle(
            position.X,
            position.Y + width,
            position.X + size.X,
            position.Y + size.Y));
}
