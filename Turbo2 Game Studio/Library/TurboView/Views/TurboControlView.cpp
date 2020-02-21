
#include <pch.h>

#include <TurboControlView.h>

using namespace Turbo::View;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

TurboControlView::TurboControlView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<ITurboControlViewModel> controlViewModel) :
        TurboView(name),
        _rendererAccess(std::move(rendererAccess)),
        _controlViewModel(std::move(controlViewModel))
{
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void TurboControlView::Load()
{
    auto sprite = _controlViewModel->Sprite();
    if (sprite == nullptr)
        return;

    _rendererAccess->LoadSceneSprite(sprite);
}

void TurboControlView::Render()
{
    auto sprite = _controlViewModel->Sprite();
    if (sprite == nullptr)
        return;

    _rendererAccess->RenderSceneSprite(sprite);
}

//	ITurboControlView Properties ---------------------------------------------------------------------------------------

bool TurboControlView::IsActive()
{
    return _controlViewModel->IsActive();
}

void TurboControlView::IsActive(bool isActive)
{
    _controlViewModel->IsActive(isActive);
}

//	ITurboControlView Methods ------------------------------------------------------------------------------------------

void TurboControlView::CurrentPoint(float x, float y, float z)
{
    _controlViewModel->CurrentPoint(x, y, z);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void TurboControlView::UpdateLayout(TurboVector2D position, TurboVector2D size)
{
    auto sprite = _controlViewModel->Sprite();
    if (sprite == nullptr)
        return;

    sprite->UseRectangle(true);
    sprite->Left(position.X);
    sprite->Top(position.Y);
    sprite->Right(position.X + size.X);
    sprite->Bottom(position.Y + size.Y);
}
