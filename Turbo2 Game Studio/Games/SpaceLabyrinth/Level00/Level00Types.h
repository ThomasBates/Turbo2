
#pragma once

#include <pch.h>

struct Level00MazeOptions
{
	int		LevelRound;
	UINT	MazeSize;
	UINT	KeyCount;
	UINT	RequiredKeyCount;
	UINT	HazardCount;
	bool	MovingKeys;
	bool	MovingHazards;
	bool	LightsOn;
};


struct Level00UserOptions
{
	bool	InvertedMouse;
	bool	SoundEffectsOn;
};