
#pragma once

namespace Turbo
{
	namespace Math
	{
		struct TurboVector2D
		{
			double X, Y;

			//  Constructors
			TurboVector2D();
			TurboVector2D(double x, double y);

			double Length();
			TurboVector2D Normalize();

			TurboVector2D operator-() const;

			TurboVector2D operator+(const TurboVector2D &v) const;
			TurboVector2D operator-(const TurboVector2D &v) const;
			TurboVector2D operator*(double r) const;
			TurboVector2D operator/(double r) const;

			TurboVector2D &operator=(double r);
			TurboVector2D &operator+=(const TurboVector2D &v);
			TurboVector2D &operator-=(const TurboVector2D &v);
			TurboVector2D &operator*=(double r);
			TurboVector2D &operator/=(double r);

			double operator*(const TurboVector2D &v) const;   //  Dot product
		};

		inline std::ostream& operator<<(std::ostream& os, const TurboVector2D& v)
		{
			os << "(" << v.X << "," << v.Y << ")";
			return os;
		}
	}
}
