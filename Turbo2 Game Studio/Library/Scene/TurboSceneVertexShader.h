#pragma once

#include <ITurboSceneVertexShader.h>

class TurboSceneVertexShader : public ITurboSceneVertexShader
{
public:
	//  Constructors  --------------------------------------------------------------------------------------------------
	TurboSceneVertexShader(std::string name);

	//  ITurboSceneObjectTexture Properties  ---------------------------------------------------------------------------
	virtual std::string Name() { return _name; }

private:
	std::string _name;
};