
#include <pch.h>

#include <TurboDialogView.h>

#include <TurboSceneSprite.h>
#include <TurboSceneText.h>
#include <TurboSceneTexture.h>
#include <TurboSceneView.h>
#include <Views/TurboControlView.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

TurboDialogView::TurboDialogView(
        std::shared_ptr<ITurboDebug> debug,
        std::string name,
        const std::shared_ptr<ITurboViewRendererAccess>& rendererAccess,
        std::shared_ptr<TurboDialogViewModel> viewModel) :
        TurboGroupView(std::move(name), rendererAccess),
        _debug(std::move(debug)),
        _viewModel(std::move(viewModel))
{
    //_sceneView = std::shared_ptr<ITurboView>(new TurboSceneView("Main Scene View", rendererAccess, _viewModel->MainSceneViewModel()));
    //InternalAddView(_sceneView);

    auto texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("LaboratoryWall"));
    _background = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));

    _okControlView = std::shared_ptr<ITurboView>(new TurboControlView("OK Button View", rendererAccess, _viewModel->OKViewModel(), "TextButton"));
    InternalAddView(_okControlView);
    _okText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 120, TurboColor(0.3F, 0.3F, 0.3F), "OK"));
    _okText->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);

    if (_viewModel->ShowCancel())
    {
        _cancelControlView = std::shared_ptr<ITurboView>(new TurboControlView("Cancel Button View", rendererAccess, _viewModel->CancelViewModel(), "TextButton"));
        InternalAddView(_cancelControlView);
        _cancelText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 120, TurboColor(0.3F, 0.3F, 0.3F), "Cancel"));
        _cancelText->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);
    }

    _captionText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 80, TurboColor(0.3F, 0.3F, 0.3F), _viewModel->CaptionText()));
    _captionText->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);

    _dialogText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(0.5F, 0.5F, 0.5F), _viewModel->DialogText()));
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void TurboDialogView::Load()
{
    LoadSceneSprite(_background);

    TurboGroupView::Load();

    _captionText->Text(_viewModel->CaptionText());
    _dialogText->Text(_viewModel->DialogText());

    LoadSceneText(_captionText);
    LoadSceneText(_dialogText);

    LoadSceneText(_okText);
    if (_viewModel->ShowCancel())
        LoadSceneText(_cancelText);
}

void TurboDialogView::Render()
{
    RenderSceneSprite(_background);

    TurboGroupView::Render();

    RenderSceneText(_captionText);
    RenderSceneText(_dialogText);

    RenderSceneText(_okText);
    if (_viewModel->ShowCancel())
        RenderSceneText(_cancelText);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void TurboDialogView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _debug->Send(debugDebug, debugView) << "TurboDialogView::UpdateLayout: position = " << position << ", size = " << size << "\n";

    //  main scene (whole screen)
    //_sceneView->Position(position);
    //_sceneView->Size(size);

    _background->Rectangle(TurboRectangle(
            position.X,
            position.Y,
            position.X + size.X,
            position.Y + size.Y));

    float width = (size.X < size.Y) ? size.X / 8 : size.Y / 8;

    _captionText->Rectangle(TurboRectangle(
            position.X,
            position.Y,
            position.X + size.X,
            position.Y + width));

    _dialogText->Rectangle(TurboRectangle(
            position.X,
            position.Y + width,
            position.X + size.X,
            position.Y + size.Y - width * 2));

    TurboVector2D center { position.X + (size.X - position.X) / 2, position.Y + size.Y - width };

    if (_viewModel->ShowCancel())
    {
        _okControlView->Position(TurboVector2D(center.X - width * 3.5f, center.Y - width * 0.5f));
        _okControlView->Size(TurboVector2D(width * 3, width));
        _okText->Rectangle(TurboRectangle(
                center.X - width * 3.5f,
                center.Y - width * 0.5f,
                center.X - width * 0.5f,
                center.Y + width * 0.5f));

        _cancelControlView->Position(TurboVector2D(center.X + width * 0.5f, center.Y - width * 0.5f));
        _cancelControlView->Size(TurboVector2D(width * 3, width));
        _cancelText->Rectangle(TurboRectangle(
                center.X + width * 0.5f,
                center.Y - width * 0.5f,
                center.X + width * 3.5f,
                center.Y + width * 0.5f));
    }
    else
    {
        _okControlView->Position(TurboVector2D(center.X - width * 1.5f, center.Y - width * 0.5f));
        _okControlView->Size(TurboVector2D(width * 3, width));
        _okText->Rectangle(TurboRectangle(
                center.X - width * 1.5f,
                center.Y - width * 0.5f,
                center.X + width * 1.5f,
                center.Y + width * 0.5f));
    }
}
