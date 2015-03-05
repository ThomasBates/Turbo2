
#pragma once

#include "ApplicationNDS.h"
#include "IImage.h"
#include "ISpaceLabyrinthFactory.h"

class SpaceLabyrinthNDS : public ISpaceLabyrinthFactory
{
private:
	ApplicationNDS *_application;
	int				_texture[6];
	float			_camera[4][4];
	unsigned int	_ticks;
	float			_time;
	float			_deltaTime;

public:
	//  Constructors and Destructors
	SpaceLabyrinthNDS(ApplicationNDS *application);
	~SpaceLabyrinthNDS();

	//  ISpaceLabyrinthFactory Methods
	virtual int Initialize();
//	virtual int Reset();
	virtual int Resize(int width, int height);
	virtual int BeginUpdate();
	virtual int EndUpdate();
	virtual int Finalize();

	virtual int DrawWall(float left, float top, float back, float right, float bottom, float front);
	virtual int GetNavigationInfo(NavInfo *navInfo);
	virtual float GetTime() { return _time; }
	virtual float GetDeltaTime() { return _deltaTime; }
	virtual int MoveCamera(float x, float y, float z);
	virtual int RotateCamera(float x, float y, float z);

protected:
	//  Local Support Methods
	virtual void InitSubSprites();
	virtual int LoadTextures();
	virtual int LoadTexture(int id, const u8 *imageData);
	virtual IImage *LoadImage(const u8 *imageData);
};