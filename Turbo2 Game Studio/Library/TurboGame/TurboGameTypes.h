#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Game
	{
		enum TurboGameLevelState
		{
			Initializing,
			Running,
			Failing,
			Failed,
			Completed,
			LevelStateCount
		};
	}
}