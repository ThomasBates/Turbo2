
#pragma once

class IGameLevel
{
public:
	virtual int		Initialize() = 0;
	virtual int		Update() = 0;
	virtual int		Render() = 0;
	virtual int		SaveState() = 0;
	virtual int		Finalize() = 0;
};