
#include <pch.h>

#include <MazePreviewMenuView.h>
#include <MazePreviewHUD1View.h>
#include <MazePreviewHUD2View.h>
#include <MazePreviewMainControlView.h>
#include <MazePreviewMotionControlView.h>
#include <MazePreviewDirectionControlView.h>

#include <TurboSceneSprite.h>
#include <TurboSceneTexture.h>
#include <TurboSceneView.h>
#include <TurboSceneSound.h>
#include <Views/TurboControlView.h>
#include <TurboSceneText.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewMenuView::MazePreviewMenuView(
        std::shared_ptr<ITurboDebug> debug,
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewMenuViewModel> viewModel) :
        TurboDialogView(debug, name, rendererAccess, viewModel),
        _debug(debug),
        _viewModel(viewModel)
{
    _reverseControlsText = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 100, TurboColor(), "Reverse Controls"));
    _soundEffectsText    = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 100, TurboColor(), "Sound Effects"));
    _resetProgressText   = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 100, TurboColor(), "Reset Progress"));
    _selectRoundText     = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 100, TurboColor(), "Select Round"));
    _selectMazeText      = std::shared_ptr<ITurboSceneText>(new TurboSceneText("Arial", 100, TurboColor(), "Select Maze"));

    _reverseControlToggleView = std::shared_ptr<TurboToggleControlView>(new TurboToggleControlView("Reverse Control Toggle View", rendererAccess, _viewModel->ReverseControlViewModel()));
    _soundEffectsToggleView = std::shared_ptr<TurboToggleControlView>(new TurboToggleControlView("Sound Effects Toggle View", rendererAccess, _viewModel->SoundEffectsViewModel()));

    InternalAddView(_reverseControlToggleView);
    InternalAddView(_soundEffectsToggleView);
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewMenuView::Load()
{
    TurboDialogView::Load();

    LoadSceneText(_reverseControlsText);
    LoadSceneText(_soundEffectsText   );
    LoadSceneText(_resetProgressText  );
    LoadSceneText(_selectRoundText    );
    LoadSceneText(_selectMazeText     );
}

void MazePreviewMenuView::Render()
{
    TurboDialogView::Render();

    RenderSceneText(_reverseControlsText);
    RenderSceneText(_soundEffectsText   );
    RenderSceneText(_resetProgressText  );
    RenderSceneText(_selectRoundText    );
    RenderSceneText(_selectMazeText     );
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewMenuView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _debug->Send(debugDebug, debugView) << "MazePreviewMenuView::UpdateLayout: position = " << position << ", size = " << size << "\n";

    TurboDialogView::UpdateLayout(position, size);

    //auto textColor = TurboColor(1.0F, 1.0F, 1.0F);
    auto textColor = TurboColor(0.3F, 0.3F, 0.3F);

    _reverseControlsText->Color(textColor);
    _soundEffectsText   ->Color(textColor);
    _resetProgressText  ->Color(textColor);
    _selectRoundText    ->Color(textColor);
    _selectMazeText     ->Color(textColor);

    float width = (size.X < size.Y) ? size.X / 8 : size.Y / 8;

    if (size.X < size.Y)    //  Portrait layout
    {
        _reverseControlsText->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);
        _soundEffectsText   ->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);
        _resetProgressText  ->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);
        _selectRoundText    ->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);
        _selectMazeText     ->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalCenter);

        float smallSpacing = 1.0f;
        float bigSpacing = 1.0f;

        float x1 = position.X;
        float x2 = position.X + size.X;
        float y1 = position.Y + (size.Y - width) / 2 - (width * smallSpacing * 5 + width * bigSpacing * 4 + width) / 2;
        float y2 = y1 + width;

        _reverseControlsText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        y1 += width * smallSpacing;
        y2 += width * smallSpacing;
        _reverseControlToggleView->Position(TurboVector2D(x1 + (x2 - x1)/2 - width, y1));
        _reverseControlToggleView->Size(TurboVector2D(width * 2, width));
        y1 += width * bigSpacing;
        y2 += width * bigSpacing;

        _soundEffectsText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        y1 += width * smallSpacing;
        y2 += width * smallSpacing;
        _soundEffectsToggleView->Position(TurboVector2D(x1 + (x2 - x1)/2 - width, y1));
        _soundEffectsToggleView->Size(TurboVector2D(width * 2, width));
        y1 += width * bigSpacing;
        y2 += width * bigSpacing;

        _resetProgressText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        y1 += width * smallSpacing;
        y2 += width * smallSpacing;
        y1 += width * bigSpacing;
        y2 += width * bigSpacing;

        _selectRoundText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        y1 += width * smallSpacing;
        y2 += width * smallSpacing;
        y1 += width * bigSpacing;
        y2 += width * bigSpacing;

        _selectMazeText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        y1 += width * smallSpacing;
        y2 += width * smallSpacing;
    }
    else    //  Landscape layout
    {
        _reverseControlsText->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalRight);
        _soundEffectsText   ->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalRight);
        _resetProgressText  ->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalRight);
        _selectRoundText    ->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalRight);
        _selectMazeText     ->HorizontalAlignment(SceneTextHorizontalAlignment::horizontalRight);

        float x1 = position.X;
        float x2 = position.X + size.X / 2;
        float y1 = position.Y + size.Y / 2 - width * 3.0f;
        float y2 = y1 + width;

        _reverseControlsText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        _reverseControlToggleView->Position(TurboVector2D(x2 + width, y1));
        _reverseControlToggleView->Size(TurboVector2D(width * 2, width));
        y1 += width * 1.25f;
        y2 += width * 1.25f;

        _soundEffectsText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        _soundEffectsToggleView->Position(TurboVector2D(x2 + width, y1));
        _soundEffectsToggleView->Size(TurboVector2D(width * 2, width));
        y1 += width * 1.25f;
        y2 += width * 1.25f;

        _resetProgressText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        y1 += width * 1.25f;
        y2 += width * 1.25f;

        _selectRoundText->Rectangle(TurboRectangle(x1, y1, x2, y2));
        y1 += width * 1.25f;
        y2 += width * 1.25f;

        _selectMazeText->Rectangle(TurboRectangle(x1, y1, x2, y2));
    }
}
