
#pragma once

#include "IApplication.h"
#include "INavigationController.h"
#include "IPlatformResources.h"

#include "IMaze.h"
#include "Camera.h"

class ISpaceLabyrinthPlatform
{
public:
	//  ISpaceLabyrinthPlatform Methods
	virtual int		Initialize(Camera *camera) = 0;
	virtual void	SetPlatformResources(IPlatformResources *platformResources) = 0;
	virtual int		Resize(int width, int height) = 0;
	virtual int		BeginDraw() = 0;
	virtual int		EndDraw() = 0;
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

