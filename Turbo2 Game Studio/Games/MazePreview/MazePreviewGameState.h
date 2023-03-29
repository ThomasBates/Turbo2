
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <TurboConfigGroup.h>
#include <UserOptions.h>
#include <GameOptions.h>
#include <MazeOptions.h>

using namespace Turbo::Core::Config;
using namespace Turbo::Core::Config;

class MazePreviewGameState : public TurboConfigGroup
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	MazePreviewGameState(std::shared_ptr<ITurboDebug> debug, std::shared_ptr<ITurboConfigProvider> provider, std::string name);
	virtual ~MazePreviewGameState() {}

	virtual std::shared_ptr<UserOptions> User() { return _userOptions; }
	virtual std::shared_ptr<GameOptions> Game() { return _gameOptions; }
	virtual std::shared_ptr<MazeOptions> Maze() { return _mazeOptions; }

private:
	std::shared_ptr<UserOptions> _userOptions;
	std::shared_ptr<GameOptions> _gameOptions;
	std::shared_ptr<MazeOptions> _mazeOptions;
};
