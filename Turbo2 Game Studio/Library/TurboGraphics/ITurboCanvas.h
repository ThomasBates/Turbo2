//  ========================================================================  //
//  ICanvas.h
//  ========================================================================  //

#pragma once

#include <TurboColor.h>

//typedef	unsigned long	Color;
//typedef	unsigned char	Hue;

namespace Turbo
{
	namespace Graphics
	{
		class ITurboGraphicObject
		{
		public:
			//  Property Accessor Methods
			virtual	TurboColor	GetColor() = 0;
			virtual	void	SetColor(TurboColor color) = 0;

			//  Public Access Methods
			virtual	void	SetRGB(byte red, byte green, byte blue) = 0;
			virtual	void	SetRGB(float red, float green, float blue) = 0;
			virtual	void	SetRGBA(byte red, byte green, byte blue, byte alpha) = 0;
			virtual	void	SetRGBA(float red, float green, float blue, float alpha) = 0;
		};

		class ITurboBrush : public ITurboGraphicObject
		{
		public:
			//  Property Accessor Methods
			virtual	int		GetStyle() = 0;
			virtual	void	SetStyle(int style) = 0;
		};

		class ITurboPen : public ITurboGraphicObject
		{
		public:
			//  Property Accessor Methods
			virtual	int		Width() = 0;
			virtual	void	Width(int width) = 0;
			virtual	int		GetStyle() = 0;
			virtual	void	SetStyle(int style) = 0;
		};

		class ITurboCanvas
		{
		public:
			//  Property Accessor Methods
			virtual void*	Data() = 0;
			virtual void	Data(void* data) = 0;
			virtual int		DataSize() = 0;

			virtual int		Width() = 0;
			virtual void	Width(int width) = 0;
			virtual int		Height() = 0;
			virtual void	Height(int height) = 0;

			virtual ITurboBrush*	Brush() = 0;
			virtual void	Brush(ITurboBrush *brush) = 0;
			virtual ITurboPen*	Pen() = 0;
			virtual void	Pen(ITurboPen *pen) = 0;

			virtual int		X() = 0;
			virtual void	X(int x) = 0;
			virtual int		Y() = 0;
			virtual void	Y(int y) = 0;

			//  Public Access Methods
			virtual	TurboColor	GetPixel(int x, int y) = 0;
			virtual	void	SetPixel(int x, int y, TurboColor color) = 0;

			virtual	void	Clear() = 0;
			virtual	void	Fill() = 0;
			virtual	void	MoveTo(int x, int y) = 0;
			virtual	void	LineTo(int x, int y) = 0;

			virtual void CopyCanvas(ITurboCanvas *canvas, int left, int top, bool transparent = true) = 0;
			virtual void CopyToBuffer(void *buffer, bool transparent) = 0;
			virtual void CopyToBuffer(void *buffer, int bufferWidth, int bufferHeight, int left, int top, bool transparent = false) = 0;
		};
	}
}