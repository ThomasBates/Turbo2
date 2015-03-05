

#pragma once

#include "IWindow.h"

class Window : public IWindow
{
	//  Private Members
	IWindow		*_parent;
	int			_width;
	int			_height;

	//	Constructors
	Window(IWindow *parent);

	//  IWindow Properties
	virtual IWindow *GetParent() { return _parent; }
	virtual void SetParent(IWindow *parent) { _parent = parent; }

	virtual int GetWidth() { return _width; }
	virtual void SetWidth(int width) { _width = width; }
	virtual int GetHeight() { return _height; }
	virtual void SetHeight(int height) { _height = height; }

	//  IWindow Methods
	//  virtual int ProcessMessage();
};