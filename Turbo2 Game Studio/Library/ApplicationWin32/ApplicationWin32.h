
#pragma once

#include <stdio.h>						// Header File For Standard Input/Output ( NEW )
#include <windows.h>					// Header File For Windows

#include "IApplication.h"

class ApplicationWin32 : public IApplication
{
private:
	LPCWSTR		_appTitle;		// Application Title

	HGLRC		_hRC;			// Permanent Rendering Context
	HDC			_hDC;			// Private GDI Device Context
	HWND		_hWnd;			// Holds Our Window Handle
	HINSTANCE	_hInstance;		// Holds The Instance Of The Application
	MSG			_msg;			// Windows Message Structure
	BOOL		_fullscreen;	// Fullscreen Flag Set To Fullscreen Mode By Default

	int			_width;
	int			_height;

	BOOL		_active;		// Window Active Flag Set To TRUE By Default
	BOOL		_done;			// Bool Variable To Exit Loop

	BOOL		_keys[256];		// Array Used For The Keyboard Routine

	BOOL		_ready;
	IProgram*	_program;

public:
	//  Constructors and Destructors
	ApplicationWin32(LPCWSTR appTitle);		// Application Title
	~ApplicationWin32();

	//  Public Properties
	HGLRC GetRenderingContext() { return _hRC; }
	HDC   GetDeviceContext() { return _hDC; }

	BOOL GetActive() { return _active; }
	void SetActive(BOOL active) { _active = active; }

	BOOL GetKey(int keyIndex) { return _keys[keyIndex]; }
	void SetKey(int keyIndex, BOOL keyDown) { _keys[keyIndex] = keyDown; }

	//  IApplication Methods
	virtual BOOL Run(IProgram *program);

	//  Public Access Methods
	virtual void ProcessMessages();
	virtual void Resize(int width, int height);

protected:
	//  Local Support Methods
	virtual BOOL CreateAppWindow(int width, int height, int bits);
	//virtual BOOL InitGL();
	//virtual BOOL DrawGLScene();
	virtual void KillAppWindow();
};

extern ApplicationWin32* Win32Application;

