
#include <pch.h>

#include <TurboControlViewModel.h>
#include <TurboSceneSprite.h>
#include <TurboSceneTexture.h>

using namespace Turbo::Scene;
using namespace Turbo::View;

TurboControlViewModel::TurboControlViewModel(
        std::shared_ptr <ITurboSceneNavigationControl> control,
        std::string textureName) :
        _control(control)
{
    if (textureName.empty())
    {
        _sprite = nullptr;
        return;
    }

    auto texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture(textureName));

    _sprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite(texture));
    _sprite->UseRectangle(true);
}
