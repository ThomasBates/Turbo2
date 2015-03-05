//  ========================================================================  //
//  ICanvas.h
//  ========================================================================  //

#pragma once

typedef	unsigned long	Color;
typedef	unsigned char	Hue;

class IGraphicObject
{
public:
	//  Property Accessor Methods
	virtual	Color	GetColor() = 0;
	virtual	void	SetColor(Color color) = 0;

	//  Public Access Methods
	virtual	void	SetRGB(Color red, Color green, Color blue) = 0;
//	virtual	void	SetRGB(double red, double green, double blue) = 0;
};

class IBrush : public IGraphicObject
{
public:
	//  Property Accessor Methods
	virtual	int		GetStyle() = 0;
	virtual	void	SetStyle(int style) = 0;
};

class IPen : public IGraphicObject
{
public:
	//  Property Accessor Methods
	virtual	int		GetWidth() = 0;
	virtual	void	SetWidth(int width) = 0;
	virtual	int		GetStyle() = 0;
	virtual	void	SetStyle(int style) = 0;
};

class ICanvas
{
public:
	//  Property Accessor Methods
	virtual void*	GetData() = 0;
	virtual void	SetData(void* data) = 0;

	virtual int		GetWidth() = 0;
	virtual void	SetWidth(int width) = 0;
	virtual int		GetHeight() = 0;
	virtual void	SetHeight(int height) = 0;

	virtual IBrush*	GetBrush() = 0;
	virtual void	SetBrush(IBrush *brush) = 0;
	virtual IPen*	GetPen() = 0;
	virtual void	SetPen(IPen *pen) = 0;

	virtual int		GetX() = 0;
	virtual void	SetX(int x) = 0;
	virtual int		GetY() = 0;
	virtual void	SetY(int y) = 0;

	//  Public Access Methods
	virtual	Color	GetPixel(int x, int y) = 0;
	virtual	void	SetPixel(int x, int y, Color color) = 0;

	virtual	void	Clear() = 0;
	virtual	void	Fill() = 0;
	virtual	void	MoveTo(int x, int y) = 0;
	virtual	void	LineTo(int x, int y) = 0;
	
	virtual void CopyCanvas(ICanvas *canvas, int left, int top, bool transparent = true) = 0;
	virtual void CopyToBuffer(void *buffer, bool transparent) = 0;
	virtual void CopyToBuffer(void *buffer, int bufferWidth, int bufferHeight, int left, int top, bool transparent = false) = 0;
};
