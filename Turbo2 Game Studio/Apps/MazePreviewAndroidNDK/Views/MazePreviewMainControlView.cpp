
#include <pch.h>

#include <MazePreviewMainControlView.h>
#include <TurboControlView.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewMainControlView::MazePreviewMainControlView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewMainControlViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _viewModel(viewModel)
{
    _mainControlView = std::shared_ptr<ITurboView>(new TurboControlView("Main Control View", rendererAccess, _viewModel->ControlViewModel()));

    AddView(_mainControlView);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewMainControlView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _mainControlView->Position(position);
    _mainControlView->Size(size);
}
