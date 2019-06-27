
#pragma once

#include <pch.h>
#include <TurboMatrix4x4.h>

namespace Turbo
{
	namespace Math
	{
		struct TurboVector3D
		{
			float X, Y, Z;

			//  Constructors
			TurboVector3D();
			TurboVector3D(float x, float y, float z);

			float Length();
			TurboVector3D Normalize();

			TurboVector3D operator-() const;

			TurboVector3D operator+(const TurboVector3D &v) const;
			TurboVector3D operator-(const TurboVector3D &v) const;
			TurboVector3D operator*(float r) const;
			TurboVector3D operator/(float r) const;

			TurboVector3D &operator=(float r);
			TurboVector3D &operator+=(const TurboVector3D &v);
			TurboVector3D &operator-=(const TurboVector3D &v);
			TurboVector3D &operator*=(float r);
			TurboVector3D &operator/=(float r);

			float operator*(const TurboVector3D &v) const;   //  Dot product
			TurboVector3D operator%(const TurboVector3D &v) const;  //  cross product

			TurboVector3D operator*(const TurboMatrix4x4 &m) const;  //  transform
		};

		inline std::ostream& operator<<(std::ostream& os, const TurboVector3D& v)
		{
			os << "(" << v.X << "," << v.Y << "," << v.Z << ")";
			return os;
		}
	}
}
