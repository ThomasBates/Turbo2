
#pragma once

#include <pch.h>

#include <TurboColor.h>

namespace Turbo
{
	namespace Graphics
	{
		class ITurboGraphicObject
		{
		public:
			virtual ~ITurboGraphicObject() {}  //  need this in the interface because we're not using smart pointers here.

			//  ITurboGraphicObject Properties -------------------------------------------------------------------------
			virtual	TurboColor GetColor() = 0;
			virtual	void SetColor(TurboColor color) = 0;

			//  ITurboGraphicObject Methods ----------------------------------------------------------------------------
			virtual	void SetRGB(unsigned char red, unsigned char green, unsigned char blue) = 0;
			virtual	void SetRGB(float red, float green, float blue) = 0;
			virtual	void SetRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) = 0;
			virtual	void SetRGBA(float red, float green, float blue, float alpha) = 0;
		};
	}
}