
#include <time.h>

#include "ApplicationNDS.h"

ApplicationNDS* NDSApplication;

//  ============================================================================
//              ApplicationNDS
//  ============================================================================

//============  Constructors and Destructors  --------------------------------------------------------------------------

ApplicationNDS::ApplicationNDS()
{
	NDSApplication	= this;

	_active		= TRUE;					// Window Active Flag Set To TRUE By Default
	_done		= FALSE;

	memset(_keys, 0, sizeof(_keys));

	// Create Our OpenGL Window
	_ready		= CreateAppWindow(256,192, 16);
}

ApplicationNDS::~ApplicationNDS()
{
	KillAppWindow();								// Kill The Window
}

//----------------------------------------------------------------------------------------------------------------------
//============  IApplication Methods  ----------------------------------------------------------------------------------

int ApplicationNDS::Run(IProgram *program)
{
	if (!_ready)
		return 0;

	if (!program)
		return 0;

	_program = program;

	if (!program->Resize(256,192))
		return 0;

	if (!program->Initialize())
		return 0;

	while(!_done)								// Loop That Runs Until done=0
	{
		ProcessMessages();
	
		// Draw The Scene.
		if (!program->Update())		// Draw The Scene
			break;
	}

	// Shutdown
	return (1);							// Exit The Program
}

//----------------------------------------------------------------------------------------------------------------------
//============  Public Access Methods  ---------------------------------------------------------------------------------

void ApplicationNDS::ProcessMessages()
{
}

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

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods  ---------------------------------------------------------------------------------

int ApplicationNDS::CreateAppWindow(int width, int height, int bits)
{

	return TRUE;								// Success
}


/*

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
*/

void ApplicationNDS::KillAppWindow()							// Properly Kill The Window
{
}

//----------------------------------------------------------------------------------------------------------------------
