
#include <pch.h>

#include <TurboControlView.h>
#include <TurboSceneTexture.h>
#include <TurboSceneSprite.h>
#include <TurboToggleControlView.h>

TurboToggleControlView::TurboToggleControlView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<TurboToggleControlViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _viewModel(viewModel)
{
    _toggleControlView = std::shared_ptr<ITurboView>(new TurboControlView("Toggle Button View", rendererAccess, _viewModel->ToggleViewModel(), ""));
    InternalAddView(_toggleControlView);

    _toggleOffTexture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("ToggleOff"));
    _toggleOnTexture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("ToggleOn"));
    _toggleSprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(_toggleOffTexture));
}

void TurboToggleControlView::Load()
{
    TurboGroupView::Load();

    _viewModel->Update();

    if (_viewModel->GetValue())
        _toggleSprite->Texture(_toggleOnTexture);
    else
        _toggleSprite->Texture(_toggleOffTexture);

    LoadSceneSprite(_toggleSprite);
}

void TurboToggleControlView::Render()
{
    TurboGroupView::Render();

    RenderSceneSprite(_toggleSprite);
}

void TurboToggleControlView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _toggleControlView->Position(position);
    _toggleControlView->Size(size);

    _toggleSprite->Rectangle(TurboRectangle(
            position.X,
            position.Y,
            position.X + size.X,
            position.Y + size.Y));
}
