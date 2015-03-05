
#include <time.h>

#include "ApplicationNDS.h"

ApplicationNDS* NDSApplication;

//  ========================================================================  //
//  Classless Functions
//  ========================================================================  //

/*
LRESULT CALLBACK WndProc(	HWND	hWnd,					// Handle For This Window
							UINT	uMsg,					// Message For This Window
							WPARAM	wParam,					// Additional Message Information
							LPARAM	lParam)					// Additional Message Information
{

	switch (uMsg)								// Check For Windows Messages
	{
		case WM_ACTIVATE:						// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
				NDSApplication->SetActive(TRUE);	// Program Is Active
			else
				NDSApplication->SetActive(FALSE);	// Program Is No Longer Active

			return 0;						// Return To The Message Loop
		}

		case WM_SYSCOMMAND:						// Intercept System Commands
		{
			switch (wParam)						// Check System Calls
			{
				case SC_SCREENSAVE:				// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;					// Prevent From Happening
			}
			break;							// Exit
		}

		case WM_CLOSE:							// Did We Receive A Close Message?
		{
			PostQuitMessage(0);					// Send A Quit Message
			return 0;						// Jump Back
		}

		case WM_KEYDOWN:						// Is A Key Being Held Down?
		{
			NDSApplication->SetKey(wParam, TRUE);	// If So, Mark It As TRUE
			return 0;						// Jump Back
		}

		case WM_KEYUP:							// Has A Key Been Released?
		{
			NDSApplication->SetKey(wParam, FALSE);	// If So, Mark It As FALSE
			return 0;						// Jump Back
		}

		case WM_SIZE:							// Resize The OpenGL Window
		{
			NDSApplication->Resize(LOWORD(lParam), HIWORD(lParam));		// LoWord=Width, HiWord=Height
			return 0;						// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
*/

//  ========================================================================  //
//  Constructors and Destructors
//  ========================================================================  //

ApplicationNDS::ApplicationNDS()
{
	NDSApplication	= this;

	long now;	/*  used with "time" to randomize seed for "rand"  */
	srand( time( &now ) % 0xFFFF );

	_active		= TRUE;					// Window Active Flag Set To TRUE By Default
	_done		= FALSE;

	memset(_keys, 0, sizeof(_keys));

	// Create Our OpenGL Window
	_ready		= CreateGLWindow(640, 480, 16);
}

ApplicationNDS::~ApplicationNDS()
{
	KillGLWindow();								// Kill The Window
}

//  ========================================================================  //
//  IApplication Methods
//  ========================================================================  //

int ApplicationNDS::Run(IProgram *program)
{
	if (!_ready)
		return 0;	

	_program = program;
	_program->InitializeGraphics();

	while(!_done)								// Loop That Runs Until done=0
	{
		// Draw The Scene.
		DrawGLScene();
	}

	// Shutdown
	return (1);							// Exit The Program
}

//  ========================================================================  //
//  Public Access Methods
//  ========================================================================  //

void ApplicationNDS::Resize(int width, int height)				// Resize And Initialize The GL Window
{
	if (height==0)								// Prevent A Divide By Zero By
	{
		height=1;							// Making Height Equal One
	}

//	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(float)width/(float)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
	glLoadIdentity();							// Reset The Modelview Matrix
}

//  ========================================================================  //
//  Local Support Methods
//  ========================================================================  //

int ApplicationNDS::CreateGLWindow(int width, int height, int bits)
{

	// initialize gl
	glInit();
 
	// sub sprites hold the bottom image when 3D directed to top
	InitSubSprites();
 
	// sub background holds the top image when 3D directed to bottom
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	
	//enable textures
	glEnable(GL_TEXTURE_2D);
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call
	glViewport(0,0,255,191);
	
	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	
	// setup other material properties
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(20,20,20));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(5,5,5));
	
	
/*
	// setup the lighting
	glLight(0, RGB15(31,31,31) , 0, floattov10(-.5), floattov10(-.85));

	InitTextures();
*/
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	gluLookAt(	0.0, 0.0, 1.0,		//camera possition 
				0.0, 0.0,-2.0,		//look at
				0.0, 1.0, 0.0);		//up
	
	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0 | POLY_ID(1) ) ;
	
	glColor3f(1,1,1);

	glMatrixMode(GL_MODELVIEW);



	Resize(width, height);						// Set Up Our Perspective GL Screen

	if (!InitGL())								// Initialize Our Newly Created GL Window
	{
		KillGLWindow();							// Reset The Display
		//MessageBox(NULL,TEXT("Initialization Failed."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}


	return TRUE;								// Success
}

//-------------------------------------------------------
// set up a 2D layer construced of bitmap sprites
// this holds the image when rendering to the top screen
//-------------------------------------------------------
void ApplicationNDS::InitSubSprites()
//-------------------------------------------------------
{
	oamInit(&oamSub, SpriteMapping_Bmp_2D_256, false);
 
	int x = 0;
	int y = 0;
 
	int id = 0;

	//set up a 4x3 grid of 64x64 sprites to cover the screen
	for(y = 0; y < 3; y++)
	for(x = 0; x < 4; x++)
	{
	/*	u16 *offset = &SPRITE_GFX_SUB[(x * 64) + (y * 64 * 256)];
 
		oamSet(&oamSub, x + y * 4, x * 64, y * 64, 0, 15, SpriteSize_64x64, 
			SpriteColorFormat_Bmp, offset, -1, false,false,false,false,false);
	*/
		oamSub.oamMemory[id].attribute[0] = ATTR0_BMP | ATTR0_SQUARE | (64 * y);
		oamSub.oamMemory[id].attribute[1] = ATTR1_SIZE_64 | (64 * x);
		oamSub.oamMemory[id].attribute[2] = ATTR2_ALPHA(1) | (8 * 32 * y) | (8 * x);
		id++;
	}
 
	swiWaitForVBlank();
 
	oamUpdate(&oamSub);
}

int ApplicationNDS::InitGL()								// All Setup For OpenGL Goes Here
{

	//glShadeModel(GL_SMOOTH);						// Enables Smooth Shading

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// Black Background

	glClearDepth(1.0f);							// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations

	if (_program)
		_program->InitializeGraphics();

	return TRUE;								// Initialization Went OK
}

int ApplicationNDS::DrawGLScene()								// Here's Where We Do All The Drawing
{
	// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
	if (_active)						// Program Active?
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer

		// wait for capture unit to be ready
		while(REG_DISPCAPCNT & DCAP_ENABLE);
		
		lcdMainOnBottom();
		vramSetBankC(VRAM_C_LCD);
		vramSetBankD(VRAM_D_SUB_SPRITE);
		REG_DISPCAPCNT = DCAP_BANK(2) | DCAP_ENABLE | DCAP_SIZE(3);

		glLoadIdentity();							// Reset The Current Modelview Matrix

		if (!_program->Update(0,0))		// Draw The Scene
		{
			_done = TRUE;
			return FALSE;
		}

		//SwapBuffers(_hDC);			// Swap Buffers (Double Buffering)
	}

	return TRUE;								// Everything Went OK
}

void ApplicationNDS::KillGLWindow()							// Properly Kill The Window
{
}

