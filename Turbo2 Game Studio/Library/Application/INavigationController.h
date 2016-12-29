#pragma once

typedef struct {
	float	Time;
	float	DeltaTime;
	bool	Pointer;
	int		PointerX;
	int		PointerY;
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

class INavigationController
{
public:
	virtual void SetTimeStampForFrame() = 0;
	virtual NavigationInfo GetNavigationInfo() = 0;
};
