#pragma once

#include "pch.h"
#include <windows.h>					// Header File For Windows

#include "IProgram.h"

class IApplicationWin32
{
public:
	virtual HGLRC GetRenderingContext() = 0;
	virtual HDC   GetDeviceContext() = 0;

	virtual BOOL GetActive() = 0;
	virtual void SetActive(BOOL active) = 0;

	virtual void GetPointer(int *x, int *y, int *status) = 0;
	virtual void SetPointer(int x, int y, int status) = 0;

	virtual BOOL GetKey(int keyIndex) = 0;
	virtual void SetKey(int keyIndex, BOOL keyDown) = 0;

};

//extern IApplication *Application;
