
#include <pch.h>

#include <MazePreviewHUD2View.h>
#include <TurboControlView.h>
#include <TurboSceneTexture.h>
#include <TurboSceneSprite.h>
#include <TurboSceneText.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewHUD2View::MazePreviewHUD2View(
        std::shared_ptr<ITurboDebug> debug,
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewHUD2ViewModel> viewModel) :
        TurboGroupView(name, rendererAccess),
        _debug(debug),
        _viewModel(viewModel)
{
    _menuControlView = std::shared_ptr<ITurboView>(new TurboControlView("Menu Button View", rendererAccess, _viewModel->MenuViewModel(), "MenuButton"));
    _infoControlView = std::shared_ptr<ITurboView>(new TurboControlView("Info Button View", rendererAccess, _viewModel->InfoViewModel(), "InfoButton"));

    InternalAddView(_menuControlView);
    InternalAddView(_infoControlView);

    auto texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("Watermark"));
    _watermark = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));

    _signageText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(0.3F, 0.3F, 0.3F), _viewModel->SignageText()));
    _signageText->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalStretched);
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewHUD2View::Load()
{
    TurboGroupView::Load();

    _viewModel->Update();

    _signageText->Text(_viewModel->SignageText());

    LoadSceneSprite(_watermark);
    LoadSceneText(_signageText);
}

void MazePreviewHUD2View::Render()
{
    TurboGroupView::Render();

    RenderSceneSprite(_watermark);
    RenderSceneText(_signageText);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewHUD2View::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    float width = size.X * 0.25f;
    float height = size.Y * 0.25f;

    // watermark
    _watermark->Rectangle(TurboRectangle(
            position.X + width * 1,
            position.Y,
            position.X + width * 2,
            position.Y + height));

    _menuControlView->Position(TurboVector2D(position.X + width * 2, position.Y));
    _menuControlView->Size(TurboVector2D(width, height));

    _infoControlView->Position(TurboVector2D(position.X + width * 3, position.Y));
    _infoControlView->Size(TurboVector2D(width, height));

    // signage
    _signageText->Rectangle(TurboRectangle(
            position.X,
            position.Y + height,
            position.X + size.X - _signageText->FontSize() / 4,
            position.Y + size.Y));
}
