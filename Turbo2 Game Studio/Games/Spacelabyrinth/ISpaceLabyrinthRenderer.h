
#pragma once

//#include "IPlatformResources.h"
#include "IMaze.h"
#include "Camera.h"

//typedef struct {
//	int		Pointer;
//	int		PointerX;
//	int		PointerY;
//	int		MoveLeft;
//	int		MoveRight;
//	int		MoveDown;
//	int		MoveUp;
//	int		MoveFore;
//	int		MoveBack;
//	int		PitchFore;
//	int		PitchBack;
//	int		YawRight;
//	int		YawLeft;
//	int		RollLeft;
//	int		RollRight;
//	int		Restart;
//}	NavInfo;

class ISpaceLabyrinthRenderer
{
public:
	//  ISpaceLabyrinthRenderer Methods
//	virtual void	Initialize(Camera *camera) = 0;
	virtual void	Initialize() = 0;
//	virtual void	SetPlatformResources(IPlatformResources *platformResources) = 0;
	virtual void	Resize() = 0;
	virtual void	BeginDraw() = 0;
	virtual void	EndDraw() = 0;
	virtual void	Update(Camera *camera, double elapsedSeconds) = 0;
	virtual bool	Render() = 0;
	virtual void	Finalize() = 0;

//	virtual int		GetNavigationInfo(NavInfo *navInfo) = 0;
//	virtual float	GetTime() = 0;
//	virtual float	GetDeltaTime() = 0;

	virtual void	DrawCorner(MazeObject *corner) = 0;
	virtual void	DrawEdge(MazeObject *edge) = 0;
	virtual void	DrawWall(MazeObject *wall) = 0;
};

