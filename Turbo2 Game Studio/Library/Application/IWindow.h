
#pragma once

//#include "IApplication.h"

class IWindow
{
	virtual IWindow *GetParent() = 0;
	virtual void SetParent(IWindow *parent) = 0;

	virtual int GetWidth() = 0;
	virtual void SetWidth(int width) = 0;
	virtual int GetHeight() = 0;
	virtual void SetHeight(int height) = 0;

	//  virtual int ProcessMessage();
};

