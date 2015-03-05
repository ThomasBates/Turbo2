
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

public:
	//  Constructors and Destructors
	SpaceLabyrinthWin32(ApplicationWin32 *application);
	~SpaceLabyrinthWin32();

	//  ISpaceLabyrinthFactory Methods
	virtual int Initialize();
	virtual int BeginUpdate();
	virtual int EndUpdate();
	virtual int Resize(int width, int height);
	virtual int Reset();
	virtual int Finalize();

	virtual int DrawWall(double left, double top, double back, double right, double bottom, double front);

protected:
	//  Local Support Methods
	virtual int LoadTextures();
	virtual int LoadTexture(int id, const char *fileName);
	virtual IImage *LoadImage(const char *fileName);
};