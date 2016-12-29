
#pragma once

#include "pch.h"

#include "IProgram.h"
#include "IGameLevel.h"
#include "ITurboApplicationPlatform.h"

class SpaceLabyrinth: public IProgram
{
private:
	std::shared_ptr<ITurboApplicationPlatform>	_platform;
	std::unique_ptr<IGameLevel>					_level;

public:
	//  Constructors and Destructors
	SpaceLabyrinth(std::shared_ptr<ITurboApplicationPlatform> platform);

	//  IProgram Methods
	virtual LPCWSTR GetTitle() { return TEXT("Space Labyrinth"); }
	virtual int		Initialize();
	virtual void	SetPlatformResources(std::shared_ptr<IPlatformResources> platformResources);
	virtual int		Resize(int width, int height);
	virtual int		Update();
	virtual int		Render();
	virtual int		SaveState();
	virtual int		Finalize();

};

