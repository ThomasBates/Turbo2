
#include <pch.h>

#include <MazePreviewHUD1View.h>
#include <TurboControlView.h>
#include <TurboSceneTexture.h>
#include <TurboSceneSprite.h>

//	Constructors and Destructors ---------------------------------------------------------------------------------------

MazePreviewHUD1View::MazePreviewHUD1View(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<MazePreviewHUD1ViewModel> viewModel) :
        TurboView(name, rendererAccess),
        _viewModel(viewModel)
{
    auto texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("BackwardButton"));
    _test1 = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));

    texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("ForwardButton"));
    _test2 = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void MazePreviewHUD1View::Load()
{
    LoadSceneSprite(_test1);
    LoadSceneSprite(_test2);
}

void MazePreviewHUD1View::Render()
{
    RenderSceneSprite(_test1);
    RenderSceneSprite(_test2);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void MazePreviewHUD1View::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    float division = 0.1F;

    _test1->Left(position.X);
    _test1->Top(position.Y);
    _test1->Right(position.X + size.X);
    _test1->Bottom(position.Y + size.X * division);

    _test2->Left(position.X);
    _test2->Top(position.Y + size.X * division);
    _test2->Right(position.X + size.X);
    _test2->Bottom(position.Y + size.Y);
}
