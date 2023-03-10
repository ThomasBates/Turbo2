
#include <pch.h>

#include <MazePreviewMotionControlView.h>
#include <TurboControlView.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewMotionControlView::MazePreviewMotionControlView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewMotionControlViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _viewModel(viewModel)
{
    _forwardControlView = std::shared_ptr<ITurboView>(new TurboControlView("Forward Button View", rendererAccess, _viewModel->ForwardViewModel(), "ForwardButton"));
    _backwardControlView = std::shared_ptr<ITurboView>(new TurboControlView("Backward Button View", rendererAccess, _viewModel->BackwardViewModel(), "BackwardButton"));

    AddView(_forwardControlView);
    AddView(_backwardControlView);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewMotionControlView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _forwardControlView->Position(position);
    _forwardControlView->Size(TurboVector2D(size.X, size.Y * 0.67F));

    _backwardControlView->Position(TurboVector2D(position.X, position.Y + size.Y * 0.67F));
    _backwardControlView->Size(TurboVector2D(size.X, size.Y * 0.33F));
}
