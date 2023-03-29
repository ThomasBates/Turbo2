
#pragma once

#include <pch.h>

#include <TurboConfigGroup.h>
#include <UserOptions.h>
#include <MazeOptions.h>
#include <ITurboDebug.h>

using namespace Turbo::Core::Config;
using namespace Turbo::Core::Debug;

class GameOptions : public TurboConfigGroup
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	GameOptions(std::shared_ptr<ITurboDebug> debug, std::shared_ptr<ITurboConfigProvider> provider, std::string name);
	virtual ~GameOptions() {}

	virtual std::shared_ptr<ITurboConfigTypedItem<int>> UnlockedRound() { return _unlockedRound; }
	virtual std::shared_ptr<ITurboConfigTypedItem<int>> UnlockedLevel() { return _unlockedLevel; }
	virtual std::shared_ptr<ITurboConfigTypedItem<int>> CurrentRound() { return _currentRound; }
	virtual std::shared_ptr<ITurboConfigTypedItem<int>> CurrentLevel() { return _currentLevel; }

private:
	std::shared_ptr<ITurboConfigTypedItem<int>> _unlockedRound;
	std::shared_ptr<ITurboConfigTypedItem<int>> _unlockedLevel;
	std::shared_ptr<ITurboConfigTypedItem<int>> _currentRound;
	std::shared_ptr<ITurboConfigTypedItem<int>> _currentLevel;
};
