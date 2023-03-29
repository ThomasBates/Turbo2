
#include <pch.h>

#include <MazePreviewHUD1View.h>
#include <TurboSceneText.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewHUD1View::MazePreviewHUD1View(
        std::shared_ptr<ITurboDebug> debug,
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewHUD1ViewModel> viewModel) :
        TurboView(name, rendererAccess),
        _debug(debug),
        _viewModel(viewModel)
{
    _roundText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(0.0F, 0.0F, 1.0F), _viewModel->RoundText()));
    _titleText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(1.0F, 0.0F, 0.0F), _viewModel->TitleText()));
    _keysText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(1.0F, 0.5F, 0.0F), _viewModel->KeysText()));
    _hazardText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 60, TurboColor(1.0F, 0.0F, 0.0F), _viewModel->HazardText()));
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewHUD1View::Load()
{
    _roundText->Text(_viewModel->RoundText());
    _titleText->Text(_viewModel->TitleText());
    _keysText->Text(_viewModel->KeysText());
    _hazardText->Text(_viewModel->HazardText());

    if (_viewModel->IsUnlocked())
    {
        _keysText->Color(TurboColor(0.0F, 0.5F, 1.0F));
    }
    else
    {
        _keysText->Color(TurboColor(1.0F, 0.5F, 0.0F));
    }

    LoadSceneText(_roundText);
    LoadSceneText(_titleText);
    LoadSceneText(_keysText);
    LoadSceneText(_hazardText);
}

void MazePreviewHUD1View::Render()
{
    RenderSceneText(_roundText);
    RenderSceneText(_titleText);
    RenderSceneText(_keysText);
    RenderSceneText(_hazardText);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewHUD1View::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _debug->Send(debugDebug, debugView) << "MazePreviewHUD1View::UpdateLayout: position = " << position << ", size = " << size << "\n";

    _roundText->Rectangle(TurboRectangle(
            position.X + 15,
            position.Y + 0,
            position.X + size.X,
            position.Y + 60));

    _titleText->Rectangle(TurboRectangle(
            position.X + 15,
            position.Y + 60,
            position.X + size.X,
            position.Y + 120));

    _keysText->Rectangle(TurboRectangle(
            position.X + 15,
            position.Y + 180,
            position.X + size.X,
            position.Y + 240));

    _hazardText->Rectangle(TurboRectangle(
            position.X + 15,
            position.Y + 240,
            position.X + size.X,
            position.Y + 300));
}
