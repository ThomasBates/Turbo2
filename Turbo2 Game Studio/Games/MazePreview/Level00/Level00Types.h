
#pragma once

#include <pch.h>

struct Level00MazeOptions
{
	int		LevelRound;
	int		MazeSize;
	int		KeyCount;
	int		RequiredKeyCount;
	int		HazardCount;
	bool	MovingKeys;
	bool	MovingHazards;
	bool	LightsOn;
};


struct Level00UserOptions
{
	bool	InvertedMouse;
	bool	SoundEffectsOn;
};