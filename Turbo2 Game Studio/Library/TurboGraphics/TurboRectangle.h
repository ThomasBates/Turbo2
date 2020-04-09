
#pragma once

namespace Turbo
{
	namespace Graphics
	{
		struct TurboRectangle
		{
			float X1, Y1, X2, Y2;

			//  Constructors -------------------------------------------------------------------------------------------
			TurboRectangle()
			{
				X1 = 0;
				Y1 = 0;
				X2 = 0;
				Y2 = 0;
			}

			TurboRectangle(float x1, float y1, float x2, float y2)
			{
				X1 = x1;
				Y1 = y1;
				X2 = x2;
				Y2 = y2;
			}
		};
	}
}
