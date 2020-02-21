
#pragma once

namespace Turbo
{
	namespace Graphics
	{
		struct TurboColor
		{
			float R, G, B, A;

			//  Constructors -------------------------------------------------------------------------------------------
			TurboColor()
			{
				R = 0;
				G = 0;
				B = 0;
				A = 0;
			}

			TurboColor(float r, float g, float b)
			{
				R = r;
				G = g;
				B = b;
				A = 1;
			}

			TurboColor(float r, float g, float b, float a)
			{
				R = r;
				G = g;
				B = b;
				A = a;
			}
		};
	}
}
