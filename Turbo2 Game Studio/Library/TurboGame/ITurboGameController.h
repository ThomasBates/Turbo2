
#pragma once

typedef struct {
	double	Time;
	double	DeltaTime;
	bool	Pointer;
	float	PointerX;
	float	PointerY;
	bool	MoveLeft;
	bool	MoveRight;
	bool	MoveDown;
	bool	MoveUp;
	bool	MoveFore;
	bool	MoveBack;
	bool	PitchFore;
	bool	PitchBack;
	bool	YawRight;
	bool	YawLeft;
	bool	RollLeft;
	bool	RollRight;
	bool	Pause;
	bool	Restart;
}	NavigationInfo;

class ITurboGameController
{
public:
	virtual NavigationInfo GetNavigationInfo() = 0;
};
