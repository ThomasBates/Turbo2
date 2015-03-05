
#pragma once

class IWindow
{
	virtual int GetWidth() = 0;
	virtual void SetWidth(int width) = 0;
	virtual int GetHeight() = 0;
	virtual void SetHeight(int height) = 0;

	virtual int ProcessMessage();
};

