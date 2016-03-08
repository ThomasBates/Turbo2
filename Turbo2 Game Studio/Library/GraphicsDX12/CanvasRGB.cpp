//  ========================================================================  //
//  DSCanvasRGB.cpp
//  ========================================================================  //

#include "pch.h"

//#include <stdio.h>
//#include <memory.h>

#include "CanvasRGB.h"

#define MIN(a,b)	((a)<(b)?(a):(b))
#define MAX(a,b)	((a)>(b)?(a):(b))

//  ============================================================================
//  GraphicObjectRGB24
//  ============================================================================

void	GraphicObjectRGB::SetRGB(Color red, Color green, Color blue)
{
	_color = red | (green<<8) | (blue<<16);
}

/*
void	GraphicObjectRGB::SetRGB(double red, double green, double blue)
{
	Color r = (int)(MIN(MAX(red, 0), 1) * 255);
	Color g = (int)(MIN(MAX(red, 0), 1) * 255);
	Color b = (int)(MIN(MAX(red, 0), 1) * 255);
	_color = r & (g<<8) & (b<<16);
}
*/

//  ============================================================================
//  CanvasRGB24
//  ============================================================================

//  ============================================================================
//  Constructors & Destructors
//  ============================================================================

CanvasRGB::CanvasRGB(int width, int height)
{
	_data	= 0;
	_width	= width;
	_height	= height;

	_brush	= new BrushRGB();
	_pen	= new PenRGB();

	if (_width > 0 && _height > 0)
	{
		_data	= new char[_width * _height * 3];
		memset(_data, 0, _width * _height * 3);
	}

	MoveTo(0, 0);
}

CanvasRGB::~CanvasRGB()
{
	delete [] _data;
}

//  ============================================================================
//  Property Accessor Methods
//  ============================================================================

void	CanvasRGB::SetWidth(int width)
{
	if (width != _width)
	{
		char *data = 0;
		if (width > 0 && _height > 0)
		{
			data = new char[width * _height * 3];
			if (_width > 0)
				CopyToBuffer(data, width, _height, 0, 0);
		}
		_width = width;
		delete [] _data;
		_data = data;
	}
}

void	CanvasRGB::SetHeight(int height)
{
	if (height != _height && height > 0)
	{
		char *data = 0;
		if (_width > 0 && height > 0)
		{
			data = new char[_width * height * 3];
			if (_height > 0)
				CopyToBuffer(data, _width, height, 0, 0);
		}
		_height = height;
		delete [] _data;
		_data = data;
	}
}

//  ============================================================================
//  Public Access Methods
//  ============================================================================

/*
void CanvasRGB24::GetSize(int *width, int *height)
{
	*width	= _width;
	*height	= _height;
}

void CanvasRGB24::SetSize(int width, int height)
{
	if (_width	!= width ||
		_height	!= height)
	{
		_width	= width;
		_height	= height;

		delete [] _canvas;
		_canvas	= 0;
		
		if (_width > 0 && _height > 0)
		{
			_canvas	= new u16[_width * _height];
			memset(_canvas, 0, _width * _height * 2);
		}
	}
}

void CanvasRGB24::GetLocation(int *x, int *y)
{
	*x	= X;
	*y	= Y;
}

void CanvasRGB24::SetLocation(int x, int y)
{
	MoveTo(x, y);
}
*/

void CanvasRGB::SetPixel(int x, int y, Color color)
{
	if (x >= 0 && x < _width &&
		y >= 0 && y < _height)
	{
		int offset = (x + y * _width) * 3;
		char *pcolor = (char*)(&color);
		memcpy(_data + offset, pcolor, 3);
	}
}

Color CanvasRGB::GetPixel(int x, int y)
{
	Color color = 0;

	if (x >= 0 && x < _width &&
		y >= 0 && y < _height)
	{
		int offset = (x + y * _width) * 3;
		char *pcolor = (char*)(&color);
		memcpy(pcolor, _data + offset, 3);
	}
	return color;
}

void CanvasRGB::Clear()
{
	if (_data)
		memset(_data, 0, _width * _height * 3);
}

void CanvasRGB::Fill()
{
	Color color = _brush->GetColor();
	for (int offset = 0; offset < _width*_height*3; offset += 3)
	{
		char *pcolor = (char*)(&color);
		memcpy(_data + offset, pcolor, 3);
	}
}

void CanvasRGB::MoveTo(int x, int y)
{
	_x	= x;
	_y	= y;
}

void CanvasRGB::LineTo(int x, int y)
{
	//int palette = AddColor(Pen.Color);
	int yStep = 1;
	int xStep = 1;
	int xDiff = x - _x;
	int yDiff = y - _y;
	
	int errorTerm = 0;
	
	//need to adjust if y1 > y2
	if (yDiff < 0)
	{
		yDiff = -yDiff;		//absolute value
		yStep = -yStep;		//step up instead of down
	}
	
	//same for x
	if (xDiff < 0)
	{
		xDiff = -xDiff;
		xStep = -xStep;
	}
	
	//case for changes more in X than in Y
	if (xDiff > yDiff) 
	{
		for (int i = 0; i <= xDiff; i++)
		{
			SetPixel(_x, _y, _pen->GetColor());
			
			_x += xStep;
			
			errorTerm += yDiff;
			
			if (errorTerm > xDiff)
			{
				errorTerm -= xDiff;
				_y += yStep;
			}
		}
	}//end if xdiff > ydiff
	//case for changes more in Y than in X
	else
	{
		for (int i = 0; i <= yDiff; i++)
		{
			SetPixel(_x, _y, _pen->GetColor());
			
			_y += yStep;
			
			errorTerm += xDiff;
			
			if (errorTerm > yDiff)
			{
				errorTerm -= yDiff;
				_x += xStep;
			}
		}
	}

	_x = x;
	_x = y;
}

void CanvasRGB::CopyCanvas(ICanvas *canvas, int left, int top, bool transparent)
{
	canvas->CopyToBuffer(_data, _width, _height, left, top, transparent);
}

void CanvasRGB::CopyToBuffer(void* buffer, bool transparent)
{
	memcpy(buffer, _data, _width * _height * 3);
}

void CanvasRGB::CopyToBuffer(void* buffer, int bufferWidth, int bufferHeight, int left, int top, bool transparent)
{
	int xMin = MAX(0, -left);
	int xMax = MIN(_width, bufferWidth - left) - 1;
	int yMin = MAX(0, -top);
	int yMax = MIN(_height, bufferHeight - top) - 1;

	Color color = 0;
	char *pcolor = (char*)(&color);

	for (int y = yMin; y <= yMax; y++)
	for (int x = xMin; x <= xMax; x++)
	{
		int offset = (x + y * _width) * 3;
		memcpy(pcolor, _data + offset, 3);

		if (x >= 0 && x < bufferWidth &&
			y >= 0 && y < bufferHeight)
		{
			memcpy(((char*)buffer) + offset, pcolor, 3);
		}
	}
}
