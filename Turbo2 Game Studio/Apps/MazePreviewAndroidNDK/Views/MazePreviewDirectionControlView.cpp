
#include <pch.h>

#include <MazePreviewDirectionControlView.h>
#include <TurboControlView.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewDirectionControlView::MazePreviewDirectionControlView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewDirectionControlViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _viewModel(std::move(viewModel))
{
    _directionControlView = std::shared_ptr<ITurboView>(new TurboControlView("Direction Control View", rendererAccess, _viewModel->DirectionViewModel()));

    AddView(_directionControlView);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewDirectionControlView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _directionControlView->Position(position);
    _directionControlView->Size(size);
}
