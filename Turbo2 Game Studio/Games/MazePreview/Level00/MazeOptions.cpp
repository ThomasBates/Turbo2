
#include <pch.h>

#include <MazeOptions.h>
#include <TurboConfigTypedItem.h>
#include <TurboConfigBoolItem.h>

using namespace Turbo::Core::Config;

MazeOptions::MazeOptions(
    std::shared_ptr<ITurboDebug> debug,
    std::shared_ptr<ITurboConfigProvider> provider,
    std::string name) :
        TurboConfigGroup(std::move(provider), std::move(name))
{
    auto thisProvider = static_cast<std::shared_ptr<ITurboConfigProvider>>(this);

    _mazeSize = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "MazeSize", 3));
    _keyCount = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "KeyCount", 0));
    _requiredKeyCount = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "RequiredKeyCount", 0));
    _foundKeyCount = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "FoundKeyCount", -1));
    _hazardCount = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "HazardCount", 0));
    _movingKeys = std::shared_ptr<ITurboConfigTypedItem<bool>>(new TurboConfigBoolItem(debug, thisProvider, "MovingKeys", false));
    _movingHazards = std::shared_ptr<ITurboConfigTypedItem<bool>>(new TurboConfigBoolItem(debug, thisProvider, "MovingHazards", false));
    _lightsOn = std::shared_ptr<ITurboConfigTypedItem<bool>>(new TurboConfigBoolItem(debug, thisProvider, "LightsOn", true));

    AddItem(_mazeSize);
    AddItem(_keyCount);
    AddItem(_requiredKeyCount);
    AddItem(_foundKeyCount);
    AddItem(_hazardCount);
    AddItem(_movingKeys);
    AddItem(_movingHazards);
    AddItem(_lightsOn);
}
