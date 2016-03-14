
#pragma once

#include "IApplication.h"
#include "IDeviceResources.h"

#include "IMaze.h"
#include "Camera.h"

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

class ISpaceLabyrinthPlatform
{
public:
	//  ISpaceLabyrinthPlatform Methods
	virtual int		Initialize(Camera *camera) = 0;
	virtual void	SetDeviceResources(IDeviceResources *deviceResources) = 0;
	virtual int		Resize(int width, int height) = 0;
	virtual int		BeginUpdate() = 0;
	virtual int		EndUpdate() = 0;
	virtual int		BeginRender() = 0;
	virtual int		EndRender() = 0;
	virtual int		Finalize() = 0;

	virtual int		GetNavigationInfo(NavInfo *navInfo) = 0;
	virtual float	GetTime() = 0;
	virtual float	GetDeltaTime() = 0;

	virtual int		DrawCorner(MazeObject *corner) = 0;
	virtual int		DrawEdge(MazeObject *edge) = 0;
	virtual int		DrawWall(MazeObject *wall) = 0;
};

