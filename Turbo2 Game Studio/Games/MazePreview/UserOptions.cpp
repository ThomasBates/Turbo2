
#include <pch.h>

#include <UserOptions.h>
#include <TurboConfigTypedItem.h>
#include <TurboConfigBoolItem.h>

using namespace Turbo::Core::Config;

UserOptions::UserOptions(
    std::shared_ptr<ITurboDebug> debug,
    std::shared_ptr<ITurboConfigProvider> provider,
    std::string name) :
        TurboConfigGroup(std::move(provider), std::move(name))
{
    auto thisProvider = static_cast<std::shared_ptr<ITurboConfigProvider>>(this);

    _invertedMouse  = std::shared_ptr<ITurboConfigTypedItem<bool>>(new TurboConfigBoolItem(debug, thisProvider, "InvertedMouse", false));
    _soundEffectsOn = std::shared_ptr<ITurboConfigTypedItem<bool>>(new TurboConfigBoolItem(debug, thisProvider, "SoundEffectsOn", true));

    AddItem(_invertedMouse);
    AddItem(_soundEffectsOn);
}
