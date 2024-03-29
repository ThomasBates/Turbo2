//  ========================================================================  //
//  TurboCanvasRGB24.h
//  ========================================================================  //

#pragma once

#include <ITurboCanvas.h>

namespace Turbo
{
	namespace Graphics
	{
		class TurboGraphicObjectRGB24 : public ITurboGraphicObject
		{
		private:
			TurboColor	_color;

		public:
			//  Property Accessor Methods
			virtual	TurboColor	GetColor() { return _color; }
			virtual	void	SetColor(TurboColor color) { _color = color; }

			//  Public Access Methods
			virtual	void	SetRGB(unsigned char red, unsigned char green, unsigned char blue);
			virtual	void	SetRGB(float red, float green, float blue);
			virtual	void	SetRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
			virtual void	SetRGBA(float red, float green, float blue, float alpha);
		};

		class TurboBrushRGB24 : public ITurboBrush
		{
		private:
			TurboGraphicObjectRGB24 _go;
			int		_style;
		public:
			//  IGraphicObject Property Accessor Methods
			virtual	TurboColor	GetColor() { return _go.GetColor(); }
			virtual	void	SetColor(TurboColor color) { _go.SetColor(color); }

			//  IGraphicObject Public Access Methods
			virtual	void	SetRGB(unsigned char red, unsigned char green, unsigned char blue) { _go.SetRGB(red, green, blue); }
			virtual	void	SetRGB(float red, float green, float blue) { _go.SetRGB(red, green, blue); }
			virtual	void	SetRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) { _go.SetRGBA(red, green, blue, alpha); }
			virtual	void	SetRGBA(float red, float green, float blue, float alpha) { _go.SetRGBA(red, green, blue, alpha); }

			//  IBrush Property Accessor Methods
			virtual	int		GetStyle() { return _style; }
			virtual	void	SetStyle(int style) { _style = style; }
		};

		class TurboPenRGB24 : public ITurboPen
		{
		private:
			TurboGraphicObjectRGB24 _go;
			int		_width;
			int		_style;
		public:
			//  IGraphicObject Property Accessor Methods
			virtual	TurboColor	GetColor() { return _go.GetColor(); }
			virtual	void	SetColor(TurboColor color) { _go.SetColor(color); }

			//  IGraphicObject Public Access Methods
			virtual	void	SetRGB(unsigned char red, unsigned char green, unsigned char blue) { _go.SetRGB(red, green, blue); }
			virtual	void	SetRGB(float red, float green, float blue) { _go.SetRGB(red, green, blue); }
			virtual	void	SetRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) { _go.SetRGBA(red, green, blue, alpha); }
			virtual	void	SetRGBA(float red, float green, float blue, float alpha) { _go.SetRGBA(red, green, blue, alpha); }

			//  IPen Property Accessor Methods
			virtual	int		Width() { return _width; }
			virtual	void	Width(int width) { _width = width; }
			virtual	int		GetStyle() { return _style; }
			virtual	void	SetStyle(int style) { _style = style; }
		};

		class TurboCanvasRGB24 : public ITurboCanvas
		{
		private:
			char*	_data;
			int		_width;
			int		_height;
			ITurboBrush*	_brush;
			ITurboPen*		_pen;
			int		_x;
			int		_y;

		public:
			//  Constructors & Destructors
			TurboCanvasRGB24(int width, int height);
			~TurboCanvasRGB24();

			//  Property Accessor Methods
			virtual void*	Data() { return (void*)_data; }
			virtual void	Data(void* data) { delete[] _data; _data = (char*)data; }
			virtual int		DataSize() { return _width * _height * 3; }

			virtual	int		Width() { return _width; }
			virtual	void	Width(int width);
			virtual int		Height() { return _height; }
			virtual void	Height(int height);

			virtual ITurboBrush*	Brush() { return _brush; }
			virtual void	Brush(ITurboBrush *brush) { delete _brush; _brush = brush; }
			virtual ITurboPen*	Pen() { return _pen; }
			virtual void	Pen(ITurboPen *pen) { delete _pen; _pen = pen; }

			virtual int		X() { return _x; }
			virtual void	X(int x) { _x = x; }
			virtual int		Y() { return _y; }
			virtual void	Y(int y) { _y = y; }

			//  Public Access Methods
			virtual	TurboColor	GetPixel(int x, int y);
			virtual	void	SetPixel(int x, int y, TurboColor color);

			virtual	void	Clear();
			virtual	void	Fill();
			virtual	void	MoveTo(int x, int y);
			virtual	void	LineTo(int x, int y);

			virtual void	CopyCanvas(ITurboCanvas *canvas, int left, int top, bool transparent = true);
			virtual void	CopyToBuffer(void *buffer, bool transparent);
			virtual void	CopyToBuffer(void *buffer, int bufferWidth, int bufferHeight, int left, int top, bool transparent = false);
		};
	}
}
