//  ========================================================================  //
//  DSCanvasRGB.cpp
//  ========================================================================  //

#include <pch.h>
//#include <stdio.h>
//#include <memory.h>

#include <TurboCanvasRGB24.h>

#define MIN(a,b)	((a)<(b)?(a):(b))
#define MAX(a,b)	((a)>(b)?(a):(b))

//  ============================================================================
//  GraphicObjectRGB24
//  ============================================================================

//void	TurboGraphicObjectRGB24::SetRGB(Color red, Color green, Color blue)
//{
//	_color = red | (green<<8) | (blue<<16);
//}


void Turbo::Graphics::TurboGraphicObjectRGB24::SetRGB(unsigned char red, unsigned char green, unsigned char blue)
{
	_color = TurboColor(
		red * 1.0 / 255.0,
		green * 1.0 / 255.0,
		blue * 1.0 / 255.0,
		1.0);
}

void Turbo::Graphics::TurboGraphicObjectRGB24::SetRGB(float red, float green, float blue)
{
	_color = TurboColor(
		MIN(MAX(red, 0.0), 1.0),
		MIN(MAX(green, 0.0), 1.0),
		MIN(MAX(blue, 0.0), 1.0),
		1.0);
}

void Turbo::Graphics::TurboGraphicObjectRGB24::SetRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	_color = TurboColor(
		red * 1.0 / 255.0,
		green * 1.0 / 255.0,
		blue * 1.0 / 255.0,
		alpha * 1.0 / 255.0);
}

void Turbo::Graphics::TurboGraphicObjectRGB24::SetRGBA(float red, float green, float blue, float alpha)
{
	_color = TurboColor(
		MIN(MAX(red, 0.0), 1.0),
		MIN(MAX(green, 0.0), 1.0),
		MIN(MAX(blue, 0.0), 1.0),
		MIN(MAX(alpha, 0.0), 1.0));
}


//  ============================================================================
//  CanvasRGB24
//  ============================================================================

//  ============================================================================
//  Constructors & Destructors
//  ============================================================================

Turbo::Graphics::TurboCanvasRGB24::TurboCanvasRGB24(int width, int height)
{
	_data	= 0;
	_width	= width;
	_height	= height;

	_brush	= new TurboBrushRGB24();
	_pen	= new TurboPenRGB24();

	if (_width > 0 && _height > 0)
	{
		_data	= new char[_width * _height * 3];
		memset(_data, 0, _width * _height * 3);
	}

	MoveTo(0, 0);
}

Turbo::Graphics::TurboCanvasRGB24::~TurboCanvasRGB24()
{
	delete [] _data;
}

//  ============================================================================
//  Property Accessor Methods
//  ============================================================================

void Turbo::Graphics::TurboCanvasRGB24::Width(int width)
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

void Turbo::Graphics::TurboCanvasRGB24::Height(int height)
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

void Turbo::Graphics::TurboCanvasRGB24::SetPixel(int x, int y, TurboColor color)
{
	if (x >= 0 && x < _width &&
		y >= 0 && y < _height)
	{
		int offset = (x + y * _width) * 3;
		
		_data[offset + 0] = color.R * 255;
		_data[offset + 1] = color.G * 255;
		_data[offset + 2] = color.B * 255;
	}
}

Turbo::Graphics::TurboColor Turbo::Graphics::TurboCanvasRGB24::GetPixel(int x, int y)
{
	TurboColor color = TurboColor();

	if (x >= 0 && x < _width &&
		y >= 0 && y < _height)
	{
		int offset = (x + y * _width) * 3;

		color.R = _data[offset + 0] * 1.0 / 255.0;
		color.G = _data[offset + 1] * 1.0 / 255.0;
		color.B = _data[offset + 2] * 1.0 / 255.0;
	}

	return color;
}

void Turbo::Graphics::TurboCanvasRGB24::Clear()
{
	if (_data)
		memset(_data, 0, _width * _height * 3);
}

void Turbo::Graphics::TurboCanvasRGB24::Fill()
{
	TurboColor color = _brush->GetColor();
	for (int offset = 0; offset < _width*_height*3; offset += 3)
	{
		_data[offset + 0] = color.R * 255;
		_data[offset + 1] = color.G * 255;
		_data[offset + 2] = color.B * 255;
	}
}

void Turbo::Graphics::TurboCanvasRGB24::MoveTo(int x, int y)
{
	_x	= x;
	_y	= y;
}

void Turbo::Graphics::TurboCanvasRGB24::LineTo(int x, int y)
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

void Turbo::Graphics::TurboCanvasRGB24::CopyCanvas(ITurboCanvas *canvas, int left, int top, bool transparent)
{
	canvas->CopyToBuffer(_data, _width, _height, left, top, transparent);
}

void Turbo::Graphics::TurboCanvasRGB24::CopyToBuffer(void* buffer, bool transparent)
{
	memcpy(buffer, _data, _width * _height * 3);
}

void Turbo::Graphics::TurboCanvasRGB24::CopyToBuffer(void* buffer, int bufferWidth, int bufferHeight, int left, int top, bool transparent)
{
	int xMin = MAX(0, -left);
	int xMax = MIN(_width, bufferWidth - left) - 1;
	int yMin = MAX(0, -top);
	int yMax = MIN(_height, bufferHeight - top) - 1;

	TurboColor color = TurboColor();
	//char *pcolor = (char*)(&color);

	for (int y = yMin; y <= yMax; y++)
	for (int x = xMin; x <= xMax; x++)
	{
		int offset = (x + y * _width) * 3;
		//memcpy(pcolor, _data + offset, 3);

		if (x >= 0 && x < bufferWidth &&
			y >= 0 && y < bufferHeight)
		{
			//memcpy(((char*)buffer) + offset, pcolor, 3);
		}
	}
}
