
#include <pch.h>

#include <TurboControlView.h>
#include <TurboSceneSprite.h>
#include <TurboSceneTexture.h>

using namespace Turbo::View;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

TurboControlView::TurboControlView(
        std::string name,
        std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
        std::shared_ptr<ITurboControlViewModel> controlViewModel,
        std::string textureName) :
        TurboView(name, rendererAccess),
        _controlViewModel(std::move(controlViewModel))
{
    if (textureName.empty())
    {
        _sprite = nullptr;
        return;
    }

    auto texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture(textureName));

    _sprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void TurboControlView::Load()
{
    if (_sprite == nullptr)
        return;

    LoadSceneSprite(_sprite);
}

void TurboControlView::Render()
{
    if (_sprite == nullptr)
        return;

    RenderSceneSprite(_sprite);
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
    if (_sprite == nullptr)
        return;

    _sprite->Rectangle(TurboRectangle(
             position.X,
             position.Y,
             position.X + size.X,
             position.Y + size.Y));
}
