
#include <pch.h>

#include <MazePreviewHUD1View.h>
#include <TurboSceneText.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewHUD1View::MazePreviewHUD1View(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewHUD1ViewModel> viewModel) :
        TurboView(name, rendererAccess),
        _viewModel(viewModel)
{
    _roundText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(0.0F, 0.0F, 1.0F), _viewModel->RoundText()));
    _titleText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(1.0F, 0.0F, 0.0F), _viewModel->TitleText()));
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewHUD1View::Load()
{
    _roundText->Text(_viewModel->RoundText());
    _titleText->Text(_viewModel->TitleText());

    LoadSceneText(_roundText);
    LoadSceneText(_titleText);
}

void MazePreviewHUD1View::Render()
{
    RenderSceneText(_roundText);
    RenderSceneText(_titleText);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewHUD1View::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _roundText->UseRectangle(true);
    _roundText->Rectangle(TurboRectangle(
            position.X,
            position.Y,
            position.X + size.X,
            position.Y + 60));

    _titleText->UseRectangle(true);
    _titleText->Rectangle(TurboRectangle(
            position.X,
            position.Y + 60,
            position.X + size.X,
            position.Y + 120));
}
