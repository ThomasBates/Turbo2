
#pragma once

#include <ITurboGraphicObject.h>

namespace Turbo
{
	namespace Graphics
	{
		class ITurboPen : public ITurboGraphicObject
		{
		public:
			virtual ~ITurboPen() {}  //  need this in the interface because we're not using smart pointers here.

			//  ITurboPen Properties -----------------------------------------------------------------------------------
			virtual	int Width() = 0;
			virtual	void Width(int width) = 0;
			virtual	int GetStyle() = 0;
			virtual	void SetStyle(int style) = 0;
		};
	}
}