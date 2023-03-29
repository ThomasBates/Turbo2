
#include <pch.h>
#include <MazePreviewGameState.h>
#include <TurboConfigTypedItem.h>
#include <TurboConfigMemoryProvider.h>

MazePreviewGameState::MazePreviewGameState(
    std::shared_ptr<ITurboDebug> debug,
    std::shared_ptr<ITurboConfigProvider> provider,
    std::string name) :
        TurboConfigGroup(std::move(provider), std::move(name))
{
    auto thisProvider = static_cast<std::shared_ptr<ITurboConfigProvider>>(this);

    _userOptions = std::shared_ptr<UserOptions>(new UserOptions(debug, thisProvider, "User"));
    _gameOptions = std::shared_ptr<GameOptions>(new GameOptions(debug, thisProvider, "Game"));

    auto memoryProvider = std::shared_ptr<ITurboConfigProvider>(new TurboConfigMemoryProvider(""));

    _mazeOptions = std::shared_ptr<MazeOptions>(new MazeOptions(debug, memoryProvider, "Maze"));

    AddGroup(_userOptions);
    AddGroup(_gameOptions);
    AddGroup(_mazeOptions);
}
