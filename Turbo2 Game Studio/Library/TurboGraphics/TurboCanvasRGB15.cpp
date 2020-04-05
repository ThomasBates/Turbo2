//  ========================================================================  //
//  TurboCanvasRGB15.cpp
//  ========================================================================  //

#include <nds.h>
#include <stdio.h>

#include <TurboCanvasRGB15.h>

#define MIN(a,b)	((a)<(b)?(a):(b))
#define MAX(a,b)	((a)>(b)?(a):(b))

//  ============================================================================
//  DSGraphicsObject
//  ============================================================================

/*
void DSGraphicsObject::SetColor(u16 color)
{
	Color = color;
}

void DSGraphicsObject::SetColor32(u32 color)
{
	u8* b;
	b = (u8*)(&color);
	SetRGB(b[0], b[1], b[2]);
}

void DSGraphicsObject::SetRGB15(u8 r, u8 g, u8 b)
{
	Color = RGB15(r, g, b);
}

void DSGraphicsObject::SetRGB(u8 r, u8 g, u8 b)
{
	SetRGB15(r>>3, g>>3, b>>3);
}

u16 DSGraphicsObject::GetColor()
{
	return Color;
}

u32 DSGraphicsObject::GetColor32()
{
	u32 result = 0;
	u8* b;
	b = (u8*)(&result);
	u16 color = GetColor();
	b[0] = (color & 31) << 3;
	b[1] = (color & (31 << 5)) >> 2;
	b[2] = (color & (31 << 10)) >> 7;
	return result;
}
*/

void	TurboGraphicObjectRGB15::SetRGB(Color red, Color green, Color blue)
{
	//  Clamp the colors to the required domain.
	red   = MIN(MAX(red,   0), 0xFF);
	green = MIN(MAX(green, 0), 0xFF);
	blue  = MIN(MAX(blue,  0), 0xFF);

	_color = RGB15(red>>3, green>>3, blue>>3);
}


//  ============================================================================
//  DSCanvas
//  ============================================================================

//  ============================================================================
//  Constructors & Destructors
//  ============================================================================

TurboCanvasRGB15::TurboCanvasRGB15(int width, int height)
{
	_data	= 0;
	_width	= width;
	_height	= height;

	_brush	= new TurboBrushRGB15();
	_pen	= new TurboPenRGB15();

	if (_width > 0 && _height > 0)
	{
		_data	= new u16[_width * _height];
		memset(_data, 0, _width * _height * 2);
	}

	MoveTo(0, 0);
}

TurboCanvasRGB15::~TurboCanvasRGB15()
{
	delete [] _data;
}

//  ============================================================================
//  Property Accessor Methods
//  ============================================================================

void	TurboCanvasRGB15::SetWidth(int width)
{
	if (width != _width)
	{
		u16 *data = 0;
		if (width > 0 && _height > 0)
		{
			data = new u16[width * _height];
			if (_width > 0)
				CopyToBuffer(data, width, _height, 0, 0);
		}
		_width = width;
		delete [] _data;
		_data = data;
	}
}

void	TurboCanvasRGB15::SetHeight(int height)
{
	if (height != _height && height > 0)
	{
		u16 *data = 0;
		if (_width > 0 && height > 0)
		{
			data = new u16[_width * height * 3];
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
void TurboCanvasRGB15::GetSize(int *width, int *height)
{
	*width	= _width;
	*height	= _height;
}

void TurboCanvasRGB15::SetSize(int width, int height)
{
	if (_width	!= width ||
		_height	!= height)
	{
		_width	= width;
		_height	= height;

		delete [] _data;
		_data	= 0;
		
		if (_width > 0 && _height > 0)
		{
			_data	= new u16[_width * _height];
			memset(_data, 0, _width * _height * 2);
		}
	}
}

void TurboCanvasRGB15::GetLocation(int *x, int *y)
{
	*x	= _x;
	*y	= _y;
}

void TurboCanvasRGB15::SetLocation(int x, int y)
{
	MoveTo(x, y);
}
*/

void TurboCanvasRGB15::SetPixel(int x, int y, Color color)
{
	if (x >= 0 && x < _width &&
		y >= 0 && y < _height)
	{
		_data[x + y * _width] = (color & 0x7FFF) | BIT(15);
	}
}

Color TurboCanvasRGB15::GetPixel(int x, int y)
{
	if (x >= 0 && x < _width &&
		y >= 0 && y < _height)
	{
		return _data[x + y * _width] & 0x7FFF;
	}
	return 0;
}

void TurboCanvasRGB15::Clear()
{
	if (_data)
		memset(_data, 0, _width * _height * 2);
}

void TurboCanvasRGB15::Fill()
{
	Color color = (_brush->GetColor() & 0x7FFF) | BIT(15);
	for (int i = 0; i < _width*_height; i++)
	{
		_data[i] = color;
	}
}

void TurboCanvasRGB15::MoveTo(int x, int y)
{
	_x	= x;
	_y	= y;
}

void TurboCanvasRGB15::LineTo(int x, int y)
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
	_y = y;
}

void TurboCanvasRGB15::CopyCanvas(ITurboCanvas *canvas, int left, int top, bool transparent)
{
	canvas->CopyToBuffer(_data, _width, _height, left, top, transparent);
}

void TurboCanvasRGB15::CopyToBuffer(void* buffer, bool transparent)
{
	if (transparent)
	{
		for (int i = 0; i < _width * _height; i++)
		{
			u16 color = _data[i];
			if (color & BIT(15))
				((u16*)buffer)[i] = color;
		}
	}
	else
		memcpy(buffer, _data, _width * _height * 2);
}

void TurboCanvasRGB15::CopyToBuffer(void* buffer, int bufferWidth, int bufferHeight, int left, int top, bool transparent)
{
	int xMin = MAX(0, -left);
	int xMax = MIN(_width, bufferWidth - left) - 1;
	int yMin = MAX(0, -top);
	int yMax = MIN(_height, bufferHeight - top) - 1;

	for (int y = yMin; y <= yMax; y++)
	for (int x = xMin; x <= xMax; x++)
	{
		u16 color = _data[x + y * _width];

		if (!transparent || (color & BIT(15)))
		{
			if (x >= 0 && x < bufferWidth &&
				y >= 0 && y < bufferHeight)
			{
				((u16*)buffer)[(left + x) + (top + y) * bufferWidth] = color;
			}
		}
	}
}
