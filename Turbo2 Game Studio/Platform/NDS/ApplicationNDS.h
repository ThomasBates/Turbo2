
#pragma once

#include <nds.h>
//#include <stdlib.h>
#include <stdio.h>
//#include <nds/arm9/console.h> //basic print funcionality

#include "IApplication.h"

class ApplicationNDS : public IApplication
{
private:
	int			_active;		// Window Active Flag Set To TRUE By Default
	int			_done;			// Bool Variable To Exit Loop

	int			_keys[256];		// Array Used For The Keyboard Routine

	int			_ready;
	IProgram*	_program;
	
	int			_vBlank;

public:
	//  Constructors and Destructors
	ApplicationNDS();		// Application Title
	~ApplicationNDS();

	//  Public Properties
	int GetActive() { return _active; }
	void SetActive(int active) { _active = active; }

	int GetKey(int keyIndex) { return _keys[keyIndex]; }
	void SetKey(int keyIndex, int keyDown) { _keys[keyIndex] = keyDown; }

	//  IApplication Methods
	virtual int Run(IProgram *program);

	//  Public Access Methods
	virtual void ProcessVBlank();
	virtual void ProcessMessages();
	virtual void Resize(int width, int height);

protected:
	//  Local Support Methods
	virtual int CreateAppWindow(int width, int height, int bits);
	//virtual int InitGL();
	//virtual int DrawGLScene();
	virtual void KillAppWindow();
};

extern ApplicationNDS* NDSApplication;

