
#pragma once

#include <stdio.h>						// Header File For Standard Input/Output ( NEW )
#include <windows.h>					// Header File For Windows

#include "IApplication.h"

#define MAXWINDOWS 100

class ApplicationWin32 : public IApplication
{
private:
	LPCWSTR		_appTitle;		// Application Title

	HGLRC		_hRC;			// Permanent Rendering Context
	HDC			_hDC;			// Private GDI Device Context
	HWND		_hWnd;			// Holds Our Window Handle
	HINSTANCE	_hInstance;		// Holds The Instance Of The Application
	BOOL		_fullscreen;	// Fullscreen Flag Set To Fullscreen Mode By Default

	IWindow	   *_windows[MAXWINDOWS];
	int			_windowCount;

	int			_width;
	int			_height;

	BOOL		_active;		// Window Active Flag Set To TRUE By Default
	BOOL		_running;		// Application is running
	BOOL		_done;			// Bool Variable To Exit Loop

	BOOL		_keys[256];		// Array Used For The Keyboard Routine
	int			_pointerX;
	int			_pointerY;
	int			_pointerStatus;

	BOOL		_ready;
	//IProgram*	_program;

	LARGE_INTEGER	_frequency;
	LARGE_INTEGER	_startCount;
	LARGE_INTEGER	_lastCount;
	float			_time;
	float			_deltaTime;
	BOOL			_timeToDraw;

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
	virtual BOOL RegisterWindow(IWindow *window);
	virtual BOOL UnregisterWindow(IWindow *window);
	virtual BOOL Run();
	virtual void HandleMessage();
	virtual void ProcessMessages();

	//  Public Access Methods
	virtual LRESULT WndProc(HWND	hWnd,					// Handle For This Window
							UINT	uMsg,					// Message For This Window
							WPARAM	wParam,					// Additional Message Information
							LPARAM	lParam);				// Additional Message Information
	virtual BOOL Resize(int width, int height);
	virtual void SetPointer(int x, int y, int status);
	virtual void GetPointer(int *x, int *y, int *status);

protected:
	//  Local Support Methods
	virtual BOOL CreateAppWindow(int width, int height, int bits);
	virtual void KillAppWindow();
	virtual BOOL ProcessMessage(MSG *msg);
};

extern ApplicationWin32* Win32Application;


#define FULLSCREEN_WIDTH  800
#define FULLSCREEN_HEIGHT 600
