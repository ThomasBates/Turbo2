
#pragma once

#include "IMaze.h"

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
}	NavInfo;

typedef enum {

	ACTION_NONE			= 0x00000000,

	ACTION_MOVE_LEFT	= 0x00000001,
	ACTION_MOVE_RIGHT	= 0x00000002,
	ACTION_MOVE_DOWN	= 0x00000004,
	ACTION_MOVE_UP		= 0x00000008,
	ACTION_MOVE_FORE	= 0x00000010,
	ACTION_MOVE_BACK	= 0x00000020,

	ACTION_PITCH_FORE	= 0x00000100,
	ACTION_PITCH_BACK	= 0x00000200,
	ACTION_YAW_RIGHT	= 0x00000400,
	ACTION_YAW_LEFT		= 0x00000800,
	ACTION_ROLL_LEFT	= 0x00001000,
	ACTION_ROLL_RIGHT	= 0x00002000

} NAVIGATION_ACTION;

class ISpaceLabyrinthFactory
{
public:
	virtual int		Initialize() = 0;
//	virtual int		Reset() = 0;
	virtual int		Resize(int width, int height) = 0;
	virtual int		BeginUpdate() = 0;
	virtual int		EndUpdate() = 0;
	virtual int		Finalize() = 0;

	virtual int		DrawWall(float left, float top, float back, float right, float bottom, float front) = 0;
	virtual int		GetNavigationInfo(NavInfo *navInfo) = 0;
	virtual float	GetTime() = 0;
	virtual float	GetDeltaTime() = 0;
	virtual int		MoveCamera(float x, float y, float z) = 0;
	virtual int		RotateCamera(float x, float y, float z) = 0;
};

