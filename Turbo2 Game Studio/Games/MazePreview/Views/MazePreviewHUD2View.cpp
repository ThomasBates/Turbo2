
#include <pch.h>

#include <MazePreviewHUD2View.h>
#include <TurboControlView.h>
#include <TurboSceneTexture.h>
#include <TurboSceneSprite.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewHUD2View::MazePreviewHUD2View(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewHUD2ViewModel> viewModel) :
        TurboView(name, rendererAccess),
        _viewModel(viewModel)
{
    auto texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("Consolas"));
    _test1 = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewHUD2View::Load()
{
    //LoadSceneSprite(_test1);
}

void MazePreviewHUD2View::Render()
{
    //RenderSceneSprite(_test1);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewHUD2View::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    _test1->Rectangle(TurboRectangle(
            position.X,
            position.Y,
            position.X + size.X,
            position.Y + size.Y));
}
