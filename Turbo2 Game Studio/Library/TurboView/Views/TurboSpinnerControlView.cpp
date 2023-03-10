
#include <pch.h>

#include <TurboControlView.h>
#include <TurboSceneText.h>
#include <TurboSceneTexture.h>
#include <TurboSceneSprite.h>
#include <TurboSpinnerControlView.h>

TurboSpinnerControlView::TurboSpinnerControlView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<TurboSpinnerControlViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _viewModel(viewModel)
{
    _downButtonControlView = std::shared_ptr<ITurboView>(new TurboControlView("Spinner Down Button View", rendererAccess, _viewModel->DownButtonViewModel(), ""));
    _upButtonControlView = std::shared_ptr<ITurboView>(new TurboControlView("Spinner Up Button View", rendererAccess, _viewModel->UpButtonViewModel(), ""));
    InternalAddView(_downButtonControlView);
    InternalAddView(_upButtonControlView);

    _downButtonTexture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("SpinnerDown"));
    _upButtonTexture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("SpinnerUp"));
    _downButtonSprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(_downButtonTexture));
    _upButtonSprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(_upButtonTexture));

    _valueText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 100, TurboColor(), "1"));
    _valueText->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);
}

void TurboSpinnerControlView::Load()
{
    TurboGroupView::Load();

    _viewModel->Update();

    _valueText->Text(std::to_string(_viewModel->GetValue()));

    LoadSceneSprite(_downButtonSprite);
    LoadSceneText(_valueText);
    LoadSceneSprite(_upButtonSprite);
}

void TurboSpinnerControlView::Render()
{
    TurboGroupView::Render();

    RenderSceneSprite(_downButtonSprite);
    RenderSceneText(_valueText);
    RenderSceneSprite(_upButtonSprite);
}

void TurboSpinnerControlView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    int s = size.Y;
    _downButtonControlView->Position(position);
    //_downButtonControlView->Size(TurboVector2D(s, s));
    _downButtonControlView->Size(TurboVector2D(s/2, s));

    _downButtonSprite->Rectangle(TurboRectangle(
            position.X,
            position.Y,
            position.X + s/2,
            position.Y + s));

    _valueText->Rectangle(TurboRectangle(
            position.X + s/2,
            position.Y,
            position.X + size.X - s/2,
            position.Y + size.Y));

    //_upButtonControlView->Position(TurboVector2D(position.X + size.X - s, position.Y));
    _upButtonControlView->Position(TurboVector2D(position.X + size.X - s/2, position.Y));
    //_upButtonControlView->Size(TurboVector2D(s, s));
    _upButtonControlView->Size(TurboVector2D(s/2, s));

    _upButtonSprite->Rectangle(TurboRectangle(
            position.X + size.X - s/2,
            position.Y,
            position.X + size.X,
            position.Y + size.Y));
}
