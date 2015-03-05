
//  Adapted from Jeff Molofee's tutorial at http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=01

#include "ApplicationWin32.h"

#pragma region Classless Functions

//	IApplication		*Application;
//	ApplicationWin32	*Win32Application;

LRESULT CALLBACK WndProc(	HWND	hWnd,					// Handle For This Window
							UINT	uMsg,					// Message For This Window
							WPARAM	wParam,					// Additional Message Information
							LPARAM	lParam)					// Additional Message Information
{
	return Win32Application->WndProc(hWnd, uMsg, wParam, lParam);
}

#pragma endregion
#pragma region Constructors and Destructors

ApplicationWin32::ApplicationWin32(LPCWSTR appTitle)
{
	//	Application			= this;
	//	Win32Application	= this;

	_appTitle	= appTitle;				// Application Title

	_hRC		= NULL;					// Permanent Rendering Context
	_hDC		= NULL;					// Private GDI Device Context
	_hWnd		= NULL;					// Holds Our Window Handle
	_fullscreen	= FALSE;

	_active		= FALSE;				// Window Active Flag Set To TRUE By Default
	_done		= FALSE;

	_windowCount = 0;
	//_program	= NULL;

	memset(_keys, 0, sizeof(_keys));

	// Create Our OpenGL Window
//	_ready		= CreateApplicationWindow(800, 600, 16);
	_ready		= CreateApplicationWindow(640, 480, 16);
//	_ready		= CreateApplicationWindow(256, 192, 16);
}

ApplicationWin32::~ApplicationWin32()
{
	KillApplicationWindow();								// Kill The Window
}

#pragma endregion
#pragma region Property Accessor Methods

#pragma endregion
#pragma region IApplication Methods

BOOL ApplicationWin32::RegisterWindow(IWindow *window)
{
	if (_windowCount < MAXWINDOWS-1)
	{
		_windows[_windowCount] = window;
		_windowCount++;
	}
}

BOOL ApplicationWin32::UnregisterWindow(IWindow *window)
{
	for (int i=0; i<_windowCount; i++)
	{
		if (_windows[i] = window)
		{
			for (int j=i+1; j<_windowCount; j++)
				_windows[j-1] = _windows[j];
			_windowCount--;
			break;
		}
	}
}

BOOL ApplicationWin32::Run()
{
	if (!_ready)
		return FALSE;

	if (_windowCount < 1)
		return FALSE;

	//if (!program->Initialize())
	//	return FALSE;

	//if (!Resize(_width, _height))
	//	return FALSE;

	//QueryPerformanceFrequency(&_frequency);
	//QueryPerformanceCounter(&_startCount);
	//_lastCount	= _startCount;

	_running = TRUE;

	while(!_done)								// Loop That Runs Until done=TRUE
	{
		//LARGE_INTEGER count;
		//QueryPerformanceCounter(&count);
	
		//_time		= (float)((count.QuadPart - _startCount.QuadPart) / (double)_frequency.QuadPart);
		//_deltaTime	= (float)((count.QuadPart - _lastCount.QuadPart ) / (double)_frequency.QuadPart);
		//if (_deltaTime * 60 > 1) // [60 fps]
		//{
		//	_timeToDraw = TRUE;
		//	_lastCount	= count;
		//}

		//ProcessMessages();
		HandleMessage();

		////  Update the Scene
		//if (!program->Update())		// Draw The Scene
		//	break;

		//// Draw The Scene.
		//if (_timeToDraw)
		//{
		//	if (!program->Draw())		// Draw The Scene
		//		break;
		//	_timeToDraw = FALSE;
		//}

		//if (_keys[VK_CONTROL] && _keys[VK_RETURN])					// Is Ctrl-Enter Being Pressed?
		//{
		//	_keys[VK_RETURN]=FALSE;				// If So Make Key FALSE

		//	KillApplicationWindow();					// Kill Our Current Window
		//	_fullscreen = !_fullscreen;				// Toggle Fullscreen / Windowed Mode

		//	if (!CreateApplicationWindow(_width, _height, 16))		// Recreate Our OpenGL Window
		//		break;				// Quit If Window Was Not Created

		//	if (!Resize(_width, _height))
		//		break;
		//}
	}

	_running = FALSE;

	//program->Finalize();

	// Shutdown
	//return (0);							// Exit The Program
}

void ApplicationWin32::ProcessMessages()
{
	MSG msg;
	while (ProcessMessage(&msg));
}

void ApplicationWin32::HandleMessage()
{
	MSG msg;
	if (!ProcessMessage(&msg));
		//Idle(msg);
}


#pragma endregion
#pragma region Public Access Methods

LRESULT ApplicationWin32::WndProc(	HWND	hWnd,					// Handle For This Window
									UINT	uMsg,					// Message For This Window
									WPARAM	wParam,					// Additional Message Information
									LPARAM	lParam)					// Additional Message Information
{
	switch (uMsg)								// Check For Windows Messages
	{
		case WM_ACTIVATE:						// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))				// Check Minimization State
				SetActive(TRUE);				// Program Is Active
			else
				SetActive(FALSE);				// Program Is No Longer Active

			return 0;							// Return To The Message Loop
		}

		case WM_SYSCOMMAND:						// Intercept System Commands
		{
			switch (wParam)						// Check System Calls
			{
				case SC_SCREENSAVE:				// Screensaver Trying To Start?
				case SC_MONITORPOWER:			// Monitor Trying To Enter Powersave?
				return 0;						// Prevent From Happening
			}
			break;								// Pass to DefWindowProc
		}

		case WM_CLOSE:							// Did We Receive A Close Message?
		{
			PostQuitMessage(0);					// Send A Quit Message
			return 0;							// Jump Back
		}

		case WM_KEYDOWN:						// Is A Key Being Held Down?
		{
			SetKey(wParam, TRUE);				// If So, Mark It As TRUE
			return 0;							// Jump Back
		}

		case WM_KEYUP:							// Has A Key Been Released?
		{
			SetKey(wParam, FALSE);				// If So, Mark It As FALSE
			return 0;							// Jump Back
		}

		case WM_MOUSEMOVE:						// Has the mouse state changed?
		{
			SetPointer(	LOWORD(lParam),			// LoWord = x,
						HIWORD(lParam),			// HiWord = y,
						wParam);				// wParam = status
			return 0;							// Jump Back
		}

		case WM_SIZE:							// Resize The OpenGL Window
		{
			Resize(	LOWORD(lParam),				// LoWord = Width,
					HIWORD(lParam));			// HiWord = Height
			return 0;							// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL ApplicationWin32::Resize(int width, int height)				// Resize And Initialize The GL Window
{
	_width = width;
	_height = height;

	if (_fullscreen)
	{
		width = FULLSCREEN_WIDTH;
		height = FULLSCREEN_HEIGHT;
	}

	//if (_program)
	//	if (!_program->Resize(width, height))
	//		_done = TRUE;

	return !_done;
}

void ApplicationWin32::SetPointer(int x, int y, int status)
{
	_pointerX		= x;
	_pointerY		= y;
	_pointerStatus	= status;
}

void ApplicationWin32::GetPointer(int *x, int *y, int *status)
{
	if (x)		*x		= _pointerX;
	if (y)		*y		= _pointerY;
	if (status)	*status	= _pointerStatus;
}

#pragma endregion
#pragma region Local Support Methods

BOOL ApplicationWin32::CreateApplicationWindow(int width, int height, int bits)
{
	int			PixelFormat;						// Holds The Results After Searching For A Match
	WNDCLASS	wc;							// Windows Class Structure
	DWORD		dwExStyle;						// Window Extended Style
	DWORD		dwStyle;						// Window Style

	RECT WindowRect;							// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;						// Set Left Value To 0
	WindowRect.right=(long)width;						// Set Right Value To Requested Width
	WindowRect.top=(long)0;							// Set Top Value To 0
	WindowRect.bottom=(long)height;						// Set Bottom Value To Requested Height

	_hInstance			= GetModuleHandle(NULL);			// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Move, And Own DC For Window
	wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc Handles Messages
	wc.cbClsExtra		= 0;						// No Extra Window Data
	wc.cbWndExtra		= 0;						// No Extra Window Data
	wc.hInstance		= _hInstance;					// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;						// No Background Required For GL
	wc.lpszMenuName		= NULL;						// We Don't Want A Menu
	wc.lpszClassName	= TEXT("OpenGL");					// Set The Class Name

	if (!RegisterClass(&wc))						// Attempt To Register The Window Class
	{
		MessageBox(NULL,TEXT("Failed To Register The Window Class."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Exit And Return FALSE
	}

	if (_fullscreen)								// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;					// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= FULLSCREEN_WIDTH;			// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= FULLSCREEN_HEIGHT;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;				// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
			if (MessageBox(NULL,TEXT("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?"),TEXT("NeHe GL"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				_fullscreen=FALSE;				// Select Windowed Mode (Fullscreen=FALSE)
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,TEXT("Program Will Now Close."),TEXT("ERROR"),MB_OK|MB_ICONSTOP);
				return FALSE;					// Exit And Return FALSE
			}
		}
	}

	if (_fullscreen)								// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;					// Window Extended Style
		dwStyle=WS_POPUP;						// Windows Style
		ShowCursor(FALSE);						// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;					// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	if (!(_hWnd=CreateWindowEx(	dwExStyle,				// Extended Style For The Window
					TEXT("OpenGL"),				// Class Name
					_appTitle,					// Window Title
					WS_CLIPSIBLINGS |			// Required Window Style
					WS_CLIPCHILDREN |			// Required Window Style
					dwStyle,				// Selected Window Style
					0, 0,					// Window Position
					WindowRect.right-WindowRect.left,	// Calculate Adjusted Window Width
					WindowRect.bottom-WindowRect.top,	// Calculate Adjusted Window Height
					NULL,					// No Parent Window
					NULL,					// No Menu
					_hInstance,				// Instance
					NULL)))					// Don't Pass Anything To WM_CREATE
	{
		KillApplicationWindow();							// Reset The Display
		MessageBox(NULL,TEXT("Window Creation Error."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=					// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,						// Must Support Double Buffering
		PFD_TYPE_RGBA,							// Request An RGBA Format
		bits,								// Select Our Color Depth
		0, 0, 0, 0, 0, 0,						// Color Bits Ignored
		0,								// No Alpha Buffer
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,							// Accumulation Bits Ignored
		16,								// 16Bit Z-Buffer (Depth Buffer)
		0,								// No Stencil Buffer
		0,								// No Auxiliary Buffer
		PFD_MAIN_PLANE,							// Main Drawing Layer
		0,								// Reserved
		0, 0, 0								// Layer Masks Ignored
	};

	if (!(_hDC=GetDC(_hWnd)))							// Did We Get A Device Context?
	{
		KillApplicationWindow();							// Reset The Display
		MessageBox(NULL,TEXT("Can't Create A GL Device Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(_hDC,&pfd)))				// Did Windows Find A Matching Pixel Format?
	{
		KillApplicationWindow();							// Reset The Display
		MessageBox(NULL,TEXT("Can't Find A Suitable PixelFormat."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if(!SetPixelFormat(_hDC,PixelFormat,&pfd))				// Are We Able To Set The Pixel Format?
	{
		KillApplicationWindow();							// Reset The Display
		MessageBox(NULL,TEXT("Can't Set The PixelFormat."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if (!(_hRC=wglCreateContext(_hDC)))					// Are We Able To Get A Rendering Context?
	{
		KillApplicationWindow();							// Reset The Display
		MessageBox(NULL,TEXT("Can't Create A GL Rendering Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if(!wglMakeCurrent(_hDC, _hRC))						// Try To Activate The Rendering Context
	{
		KillApplicationWindow();							// Reset The Display
		MessageBox(NULL,TEXT("Can't Activate The GL Rendering Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	ShowWindow(_hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(_hWnd);						// Slightly Higher Priority
	SetFocus(_hWnd);								// Sets Keyboard Focus To The Window

	Resize(width, height);						// Set Up Our Perspective GL Screen

	/*
	if (!InitGL())								// Initialize Our Newly Created GL Window
	{
		KillApplicationWindow();							// Reset The Display
		MessageBox(NULL,TEXT("Initialization Failed."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}
	*/

	return TRUE;								// Success
}

void ApplicationWin32::KillApplicationWindow()							// Properly Kill The Window
{
	if (_fullscreen)								// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);						// Show Mouse Pointer
	}

	if (_hRC)								// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,TEXT("Release Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(_hRC))					// Are We Able To Delete The RC?
		{
			MessageBox(NULL,TEXT("Release Rendering Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		_hRC=NULL;							// Set RC To NULL
	}

	if (_hDC && !ReleaseDC(_hWnd, _hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,TEXT("Release Device Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		_hDC=NULL;							// Set DC To NULL
	}

	if (_hWnd && !DestroyWindow(_hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,TEXT("Could Not Release hWnd."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		_hWnd=NULL;							// Set hWnd To NULL
	}

	if (!UnregisterClass(TEXT("OpenGL"), _hInstance))				// Are We Able To Unregister Class
	{
		MessageBox(NULL,TEXT("Could Not Unregister Class."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		_hInstance=NULL;							// Set hInstance To NULL
	}
}

BOOL ApplicationWin32::ProcessMessage(MSG *msg)
{
	if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg->message != WM_QUIT)
		{
			//BOOL handled = FALSE;
			//if (_onMessage != NULL) then _onMessage(msg, handled);
			//if (!IsHintMsg(msg) && !handled && !IsMDIMsg(msg) &&
			//	!IsKeyMsg(msg) && !IsDlgMsg(msg))
			{
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}
		else
			_done = TRUE;
		return TRUE;
	}
	return FALSE;
}

#pragma endregion