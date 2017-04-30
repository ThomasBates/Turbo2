//  ========================================================================  //
//  CanvasRGB.h
//  ========================================================================  //

#pragma once

class tbGraphicsDX12TextureData
{
private:
	int		_mipLevels;

public:
	//  Property Accessor Methods
	virtual	int		GetMipLevels() { return _mipLevels; }
	virtual	void	SetColor(Color color) { _color = color; }

	//  Public Access Methods
	virtual	void	SetRGB(Color red, Color green, Color blue);
//	virtual	void	SetRGB(double red, double green, double blue);
};
