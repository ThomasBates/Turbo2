
#pragma once

#include <pch.h>

#include <ITurboGraphicObject.h>

namespace Turbo
{
	namespace Graphics
	{
		class ITurboBrush : public ITurboGraphicObject
		{
		public:
			virtual ~ITurboBrush() {}  //  need this in the interface because we're not using smart pointers here.

			//  ITurboBrush Properties --------------------------------------------------------------------------------
			virtual	int GetStyle() = 0;
			virtual	void SetStyle(int style) = 0;
		};
	}
}