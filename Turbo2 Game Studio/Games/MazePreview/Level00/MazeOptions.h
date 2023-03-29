
#pragma once

#include <pch.h>

#include <ITurboConfigTypedItem.h>
#include <TurboConfigGroup.h>
#include <ITurboDebug.h>

using namespace Turbo::Core::Config;
using namespace Turbo::Core::Debug;

class MazeOptions : public TurboConfigGroup
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazeOptions(std::shared_ptr<ITurboDebug> debug, std::shared_ptr<ITurboConfigProvider> provider, std::string name);
    virtual ~MazeOptions() {}

    virtual std::shared_ptr<ITurboConfigTypedItem<int>> MazeSize() { return _mazeSize; }
    virtual std::shared_ptr<ITurboConfigTypedItem<int>> KeyCount() { return _keyCount; }
    virtual std::shared_ptr<ITurboConfigTypedItem<int>> RequiredKeyCount() { return _requiredKeyCount; }
    virtual std::shared_ptr<ITurboConfigTypedItem<int>> FoundKeyCount() { return _foundKeyCount; }
    virtual std::shared_ptr<ITurboConfigTypedItem<int>> HazardCount() { return _hazardCount; }
    virtual std::shared_ptr<ITurboConfigTypedItem<bool>> MovingKeys() { return _movingKeys; }
    virtual std::shared_ptr<ITurboConfigTypedItem<bool>> MovingHazards() { return _movingHazards; }
    virtual std::shared_ptr<ITurboConfigTypedItem<bool>> LightsOn() { return _lightsOn; }

private:
    std::shared_ptr<ITurboConfigTypedItem<int>> _mazeSize;
    std::shared_ptr<ITurboConfigTypedItem<int>> _keyCount;
    std::shared_ptr<ITurboConfigTypedItem<int>> _requiredKeyCount;
    std::shared_ptr<ITurboConfigTypedItem<int>> _foundKeyCount;
    std::shared_ptr<ITurboConfigTypedItem<int>> _hazardCount;
    std::shared_ptr<ITurboConfigTypedItem<bool>> _movingKeys;
    std::shared_ptr<ITurboConfigTypedItem<bool>> _movingHazards;
    std::shared_ptr<ITurboConfigTypedItem<bool>> _lightsOn;
};

