
#include <pch.h>

#include <TurboSceneView.h>

using namespace Turbo::Game;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

TurboSceneView::TurboSceneView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<TurboSceneViewModel> viewModel) :
        TurboView(name),
        _rendererAccess(std::move(rendererAccess)),
        _viewModel(std::move(viewModel))
{
}

//	ITurboView Methods ------------------------------------------------------------------------------------------

void TurboSceneView::Load()
{
    _rendererAccess->LoadScene(_viewModel->Scene());
}

void TurboSceneView::Render()
{
    _rendererAccess->RenderScene(_viewModel->Scene());
}
