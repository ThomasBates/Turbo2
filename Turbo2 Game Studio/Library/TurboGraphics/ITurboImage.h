//  ========================================================================  //
//  DSBitmap.h
//  ========================================================================  //

#pragma once

#include "pch.h"

#include "ITurboCanvas.h"

namespace Turbo
{
	namespace Graphics
	{
		typedef enum
		{
			IMG_NORMAL = 0x0000,
			IMG_ZOOM = 0x0001,
			IMG_TILE = 0x0002,
			IMG_STRETCH = 0x0003,

			IMG_CENTER = 0x0000,
			IMG_LEFT = 0x0010,
			IMG_RIGHT = 0x0020,
			IMG_TOP = 0x0040,
			IMG_BOTTOM = 0x0080,

			IMG_TRANSPARENT = 0x0100,
			IMG_ALPHA = 0x0200
		} IMG_STYLE;

		class ITurboImage
		{
		public:
			//  Property Accessor Methods
			virtual	void*	Data() = 0;
			virtual void	Data(void* data) = 0;
			virtual	ITurboCanvas *GetCanvas() = 0;
			virtual void	SetCanvas(ITurboCanvas *canvas) = 0;

			virtual int		Width() = 0;
			virtual void	Width(int width) = 0;
			virtual int		Height() = 0;
			virtual void	Height(int height) = 0;
			virtual	int		GetStyle() = 0;
			virtual void	SetStyle(int style) = 0;

			//  Methods
			//virtual int		LoadFromFile(std::string fileName) = 0;
			virtual	void	Draw(int width = -1, int height = -1, int style = 0) = 0;
		};
	}
}
