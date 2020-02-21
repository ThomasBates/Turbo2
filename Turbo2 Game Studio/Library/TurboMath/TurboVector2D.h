
#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Math
	{
		struct TurboVector2D
		{
			float X, Y;

			//  Constructors -------------------------------------------------------------------------------------------
			TurboVector2D();
			TurboVector2D(float x, float y);

			//	TurboVector2D Methods ----------------------------------------------------------------------------------
			float Length();
			TurboVector2D Normalize();

			//	TurboVector2D Operators --------------------------------------------------------------------------------
			TurboVector2D operator-() const;

			TurboVector2D operator+(const TurboVector2D &v) const;
			TurboVector2D operator-(const TurboVector2D &v) const;
			TurboVector2D operator*(float r) const;
			TurboVector2D operator/(float r) const;

			TurboVector2D &operator=(float r);
			TurboVector2D &operator+=(const TurboVector2D &v);
			TurboVector2D &operator-=(const TurboVector2D &v);
			TurboVector2D &operator*=(float r);
			TurboVector2D &operator/=(float r);

			float operator*(const TurboVector2D &v) const;   //  Dot product
		};

		inline std::ostream& operator<<(std::ostream& os, const TurboVector2D& v)
		{
			os << "(" << v.X << "," << v.Y << ")";
			return os;
		}
	}
}
