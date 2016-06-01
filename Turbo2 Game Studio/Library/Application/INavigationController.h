#pragma once

typedef struct {
	int		Pointer;
	int		PointerX;
	int		PointerY;
	int		MoveLeft;
	int		MoveRight;
	int		MoveDown;
	int		MoveUp;
	int		MoveFore;
	int		MoveBack;
	int		PitchFore;
	int		PitchBack;
	int		YawRight;
	int		YawLeft;
	int		RollLeft;
	int		RollRight;
	int		Restart;
}	NavInfo;

class INavigationController
{
public:
	virtual int	GetNavigationInfo(NavInfo *navInfo) = 0;
};
