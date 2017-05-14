#pragma once

#include <ITurboScenePixelShader.h>

class TurboScenePixelShader : public ITurboScenePixelShader
{
public:
	//  Constructors  -----------------------------------------------------------------------------------------------------
	TurboScenePixelShader(std::string name);

	//  ITurboScenePixelShader Properties  --------------------------------------------------------------------------------
	virtual std::string Name() { return _name; }

private:
	std::string _name;
};