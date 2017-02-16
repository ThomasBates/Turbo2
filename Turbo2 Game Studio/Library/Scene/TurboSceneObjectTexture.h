#pragma once

#include <ITurboSceneObjectTexture.h>

class TurboSceneObjectTexture : public ITurboSceneObjectTexture
{
public:
	//  Constructors  --------------------------------------------------------------------------------------------------
	TurboSceneObjectTexture(std::string name);

	//  ITurboSceneObjectTexture Properties  ---------------------------------------------------------------------------
	virtual std::string Name() { return _name; }

private:
	std::string _name;
};