
#pragma once

class IProgram
{
public:
	virtual int		Initialize() = 0;
//	virtual int		Reset() = 0;
	virtual int		Resize(int width, int height) = 0;
	virtual int		Update() = 0;
	virtual int		Draw() = 0;
	virtual int		Finalize() = 0;
};