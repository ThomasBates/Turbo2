
#pragma once

#include <TurboColor.h>
#include <ITurboBrush.h>
#include <ITurboPen.h>

namespace Turbo
{
	namespace Graphics
	{
		class ITurboCanvas
		{
		public:
			virtual ~ITurboCanvas() {}  //  need this in the interface because we're not using smart pointers here.

			//  ITurboCanvas Properties --------------------------------------------------------------------------------
			virtual void* Data() = 0;
			virtual void Data(void* data) = 0;
			virtual int DataSize() = 0;

			virtual int Width() = 0;
			virtual void Width(int width) = 0;
			virtual int Height() = 0;
			virtual void Height(int height) = 0;

			virtual ITurboBrush* Brush() = 0;
			virtual void Brush(ITurboBrush *brush) = 0;
			virtual ITurboPen* Pen() = 0;
			virtual void Pen(ITurboPen *pen) = 0;

			virtual int X() = 0;
			virtual void X(int x) = 0;
			virtual int Y() = 0;
			virtual void Y(int y) = 0;

			//  ITurboCanvas Methods -----------------------------------------------------------------------------------
			virtual	TurboColor GetPixel(int x, int y) = 0;
			virtual	void SetPixel(int x, int y, TurboColor color) = 0;

			virtual	void Clear() = 0;
			virtual	void Fill() = 0;
			virtual	void MoveTo(int x, int y) = 0;
			virtual	void LineTo(int x, int y) = 0;

			virtual void CopyCanvas(ITurboCanvas *canvas, int left, int top, bool transparent = true) = 0;
			virtual void CopyToBuffer(void *buffer, bool transparent) = 0;
			virtual void CopyToBuffer(void *buffer, int bufferWidth, int bufferHeight, int left, int top, bool transparent = false) = 0;
		};
	}
}