//  ========================================================================  //
//  CanvasRGB.h
//  ========================================================================  //

#pragma once

#include "ITurboCanvas.h"

class TurboGraphicObjectRGB15 : public ITurboGraphicObject
{
private:
	Color	_color;

public:
	//  Property Accessor Methods
	virtual	Color	GetColor() { return _color; }
	virtual	void	SetColor(Color color) { _color = color; }

	//  Public Access Methods
	virtual	void	SetRGB(Color red, Color green, Color blue);
//	virtual	void	SetRGB(double red, double green, double blue);
};

class TurboBrushRGB15 : public ITurboBrush
{
private:
	TurboGraphicObjectRGB15 _go;
	int		_style;
public:
	//  IGraphicObject Property Accessor Methods
	virtual	Color	GetColor() { return _go.GetColor(); }
	virtual	void	SetColor(Color color) { _go.SetColor(color); }

	//  IGraphicObject Public Access Methods
	virtual	void	SetRGB(Color red, Color green, Color blue) { _go.SetRGB(red, green, blue); }
//	virtual	void	SetRGB(double red, double green, double blue) { _go.SetRGB(red, green, blue); }

	//  IBrush Property Accessor Methods
	virtual	int		GetStyle() { return _style; }
	virtual	void	SetStyle(int style) { _style = style; }
};

class TurboPenRGB15 : public ITurboPen
{
private:
	TurboGraphicObjectRGB15 _go;
	int		_width;
	int		_style;
public:
	//  IGraphicObject Property Accessor Methods
	virtual	Color	GetColor() { return _go.GetColor(); }
	virtual	void	SetColor(Color color) { _go.SetColor(color); }

	//  IGraphicObject Public Access Methods
	virtual	void	SetRGB(Color red, Color green, Color blue) { _go.SetRGB(red, green, blue); }
//	virtual	void	SetRGB(double red, double green, double blue) { _go.SetRGB(red, green, blue); }

	//  IPen Property Accessor Methods
	virtual	int		GetWidth() { return _width; }
	virtual	void	SetWidth(int width) { _width = width; }
	virtual	int		GetStyle() { return _style; }
	virtual	void	SetStyle(int style) { _style = style; }
};

class TurboCanvasRGB15 : public ITurboCanvas
{
private:
	unsigned short*	_data;
	int		_width;
	int		_height;
	ITurboBrush*	_brush;
	ITurboPen*		_pen;
	int		_x;
	int		_y;

public:
	//  Constructors & Destructors
	TurboCanvasRGB15(int width, int height);
	~TurboCanvasRGB15();

	//  Property Accessor Methods
	virtual void*	GetData() { return (void*)_data; }
	virtual void	SetData(void* data) { delete [] _data; _data = (unsigned short*)data; }

	virtual	int		GetWidth() { return _width; }
	virtual	void	SetWidth(int width);
	virtual int		GetHeight() { return _height; }
	virtual void	SetHeight(int height);

	virtual ITurboBrush*	GetBrush() { return _brush; }
	virtual void	SetBrush(ITurboBrush *brush) { delete _brush; _brush = brush; }
	virtual ITurboPen*	GetPen() { return _pen; }
	virtual void	SetPen(ITurboPen *pen) { delete _pen; _pen = pen; }

	virtual int		GetX() { return _x; }
	virtual void	SetX(int x) { _x = x; }
	virtual int		GetY() { return _y; }
	virtual void	SetY(int y) { _y = y; }

	//  Public Access Methods
	virtual	Color	GetPixel(int x, int y);
	virtual	void	SetPixel(int x, int y, Color color);

	virtual	void	Clear();
	virtual	void	Fill();
	virtual	void	MoveTo(int x, int y);
	virtual	void	LineTo(int x, int y);
	
	virtual void	CopyCanvas(ITurboCanvas *canvas, int left, int top, bool transparent = true);
	virtual void	CopyToBuffer(void *buffer, bool transparent);
	virtual void	CopyToBuffer(void *buffer, int bufferWidth, int bufferHeight, int left, int top, bool transparent = false);
};

