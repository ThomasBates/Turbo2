
#pragma once

#include "IApplicationWin32.h"

#include "IApplication.h"
#include "IImage.h"
#include "ISpaceLabyrinthPlatform.h"

#include <gl\gl.h>						// Header File For The OpenGL32 Library
#include <gl\glu.h>						// Header File For The GLu32 Library

class SpaceLabyrinthWin32Platform : public ISpaceLabyrinthPlatform
{
private:
	IApplicationWin32 *_applicationWin32;

	HGLRC			_hRC;			// Permanent Rendering Context
	HDC				_hDC;			// Private GDI Device Context

	int				_width;
	int				_height;

	GLuint			_texture[6];
	Camera			*_camera;

	LARGE_INTEGER	_frequency;
	LARGE_INTEGER	_startCount;
	LARGE_INTEGER	_lastCount;
	float			_time;
	float			_deltaTime;

public:
	//  Constructors and Destructors
	SpaceLabyrinthWin32Platform(IApplication *application);
	~SpaceLabyrinthWin32Platform();

	//  ISpaceLabyrinthPlatform Methods
	virtual BOOL	Initialize(Camera *camera);
//	virtual BOOL	Resize(int width, int height);
	virtual BOOL	BeginUpdate();
	virtual BOOL	EndUpdate();
	virtual BOOL	BeginDraw();
	virtual BOOL	EndDraw();
	virtual BOOL	Finalize();

	virtual BOOL	GetNavigationInfo(NavInfo *navInfo);
	virtual float	GetTime() { return _time; }
	virtual float	GetDeltaTime() { return _deltaTime; }

	virtual BOOL	DrawCorner(MazeObject *corner);
	virtual BOOL	DrawEdge(MazeObject *edge);
	virtual BOOL	DrawWall(MazeObject *wall);

protected:
	//  Local Support Methods
	virtual int LoadTextures();
	virtual int LoadTexture(int id, const char *fileName);
	virtual IImage *LoadImage(const char *fileName);
	virtual void SetCornerTexture();
	virtual void SetEdgeTexture();
	virtual void SetWallTexture();
	virtual void SetCeilingTexture();
	virtual void SetFloorTexture();
};