
#include <pch.h>
#include <GameOptions.h>
#include <TurboConfigTypedItem.h>
#include <TurboConfigMemoryProvider.h>

GameOptions::GameOptions(
    std::shared_ptr<ITurboDebug> debug,
    std::shared_ptr<ITurboConfigProvider> provider,
    std::string name) :
    TurboConfigGroup(std::move(provider), std::move(name))
{
    auto thisProvider = static_cast<std::shared_ptr<ITurboConfigProvider>>(this);

    _unlockedRound = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "UnlockedRound", 1));
    _unlockedLevel = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "UnlockedLevel", 1));
    _currentRound  = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "CurrentRound", 1));
    _currentLevel  = std::shared_ptr<ITurboConfigTypedItem<int>>(new TurboConfigTypedItem<int>(debug, thisProvider, "CurrentLevel", 1));

    AddItem(_unlockedRound);
    AddItem(_unlockedLevel);
    AddItem(_currentRound );
    AddItem(_currentLevel );
}
