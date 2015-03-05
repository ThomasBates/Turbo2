
#pragma once

#include "ApplicationWin32.h"
#include "IImage.h"
#include "ISpaceLabyrinthFactory.h"

#include <gl\gl.h>						// Header File For The OpenGL32 Library
#include <gl\glu.h>						// Header File For The GLu32 Library

class SpaceLabyrinthWin32 : public ISpaceLabyrinthFactory
{
private:
	ApplicationWin32 *_application;

	HGLRC		_hRC;			// Permanent Rendering Context
	HDC			_hDC;			// Private GDI Device Context

	int	_width;
	int	_height;

	GLuint _texture[6];
	unsigned int	_ticks;
	float			_time;
	float			_deltaTime;

public:
	//  Constructors and Destructors
	SpaceLabyrinthWin32(ApplicationWin32 *application);
	~SpaceLabyrinthWin32();

	//  ISpaceLabyrinthFactory Methods
	virtual int Initialize();
//	virtual int Reset();
	virtual int Resize(int width, int height);
	virtual int BeginUpdate();
	virtual int EndUpdate();
	virtual int Finalize();

	virtual BOOL DrawWall(float left, float top, float back, float right, float bottom, float front);
	virtual BOOL GetNavigationInfo(NavInfo *navInfo);
	virtual float GetTime() { return _time; }
	virtual float GetDeltaTime() { return _deltaTime; }
	virtual int MoveCamera(float x, float y, float z);
	virtual int RotateCamera(float x, float y, float z);

protected:
	//  Local Support Methods
	virtual int LoadTextures();
	virtual int LoadTexture(int id, const char *fileName);
	virtual IImage *LoadImage(const char *fileName);
};