
#pragma once

#include <pch.h>

struct Level00GameOptions
{
	bool 	OptionsChanged;
	int		UnlockedRound;
	int		UnlockedLevel;
	int		CurrentRound;
	int 	CurrentLevel;
};

struct Level00MazeOptions
{
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