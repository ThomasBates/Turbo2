
#pragma once

#include "ApplicationNDS.h"
#include "IImage.h"
#include "ISpaceLabyrinthFactory.h"

class SpaceLabyrinthNDS : public ISpaceLabyrinthFactory
{
private:
	ApplicationNDS *_application;
	int				_texture[6];
	Camera		   *_camera;
	unsigned int	_ticks;
	float			_time;
	float			_deltaTime;

public:
	//  Constructors and Destructors
	SpaceLabyrinthNDS(ApplicationNDS *application);
	~SpaceLabyrinthNDS();

	//  ISpaceLabyrinthFactory Methods
	virtual int Initialize(Camera *camera);
	virtual int Resize(int width, int height);
	virtual int BeginUpdate();
	virtual int EndUpdate();
	virtual int BeginDraw();
	virtual int EndDraw();
	virtual int Finalize();

	virtual	int		DrawCorner(MazeObject *corner);
	virtual	int		DrawEdge(MazeObject *edge);
	virtual	int		DrawWall(MazeObject *wall);
	virtual int		GetNavigationInfo(NavInfo *navInfo);
	virtual float	GetTime() { return _time; }
	virtual float	GetDeltaTime() { return _deltaTime; }

protected:
	//  Local Support Methods
	virtual void InitSubSprites();
	virtual int LoadTextures();
	virtual int LoadTexture(int id, const u8 *imageData);
	virtual IImage *LoadImage(const u8 *imageData);
	virtual void SetCornerTexture();
	virtual void SetEdgeTexture();
	virtual void SetWallTexture();
	virtual void SetCeilingTexture();
	virtual void SetFloorTexture();
	virtual void SetVertex(float x, float y, float z, float u, float v);
};