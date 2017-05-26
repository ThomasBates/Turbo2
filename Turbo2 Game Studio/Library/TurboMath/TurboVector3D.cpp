
#include "pch.h"
//#include <math.h>
#include "TurboVector3D.h"

Turbo::Math::TurboVector3D::TurboVector3D()
{
	X = 0;
	Y = 0;
	Z = 0;
}

Turbo::Math::TurboVector3D::TurboVector3D( double x, double y, double z )
{
	X = x;
	Y = y;
	Z = z;
}

double Turbo::Math::TurboVector3D::Length()
{
	return (double)sqrt(X*X + Y*Y + Z*Z);
}

Turbo::Math::TurboVector3D Turbo::Math::TurboVector3D::Normalize()
{
	return *this / Length();
}

Turbo::Math::TurboVector3D Turbo::Math::TurboVector3D::operator-() const
{
	return TurboVector3D(-X, -Y, -Z);
}

Turbo::Math::TurboVector3D Turbo::Math::TurboVector3D::operator+(const TurboVector3D &v) const
{
	return TurboVector3D(X + v.X, Y + v.Y, Z + v.Z);
}

Turbo::Math::TurboVector3D Turbo::Math::TurboVector3D::operator-(const TurboVector3D &v) const
{
	return TurboVector3D(X - v.X, Y - v.Y, Z - v.Z);
}

Turbo::Math::TurboVector3D Turbo::Math::TurboVector3D::operator*(double r) const
{
	return TurboVector3D(X*r, Y*r, Z*r);
}

Turbo::Math::TurboVector3D Turbo::Math::TurboVector3D::operator/(double r) const
{
	if (r != 0)
		return TurboVector3D(X/r, Y/r, Z/r);
	return TurboVector3D();
}

Turbo::Math::TurboVector3D &Turbo::Math::TurboVector3D::operator=(double r)
{
	*this = TurboVector3D(r,r,r);
	return *this;
}

Turbo::Math::TurboVector3D &Turbo::Math::TurboVector3D::operator+=(const TurboVector3D &v)
{
	if (this != &v)
		*this = *this + v;
	return *this;
}

Turbo::Math::TurboVector3D &Turbo::Math::TurboVector3D::operator-=(const TurboVector3D &v)
{
	if (this != &v)
		*this = *this - v;
	return *this;
}

Turbo::Math::TurboVector3D &Turbo::Math::TurboVector3D::operator*=(double r)
{
	*this = *this * r;
	return *this;
}

Turbo::Math::TurboVector3D &Turbo::Math::TurboVector3D::operator/=(double r)
{
	*this = *this / r;
	return *this;
}

double Turbo::Math::TurboVector3D::operator*(const TurboVector3D &v) const	//dot product
{
	return X*v.X + Y*v.Y + Z*v.Z;
}

Turbo::Math::TurboVector3D Turbo::Math::TurboVector3D::operator%(const TurboVector3D &v) const	//  cross product
{
	return TurboVector3D(Y * v.Z - Z * v.Y,
					Z * v.X - X * v.Z,
					X * v.Y - Y * v.X);
}

Turbo::Math::TurboVector3D Turbo::Math::TurboVector3D::operator*(const TurboMatrix4x4 & m) const
{
	return TurboVector3D(
		X * m.M11 + Y * m.M21 + Z * m.M31 + 1 * m.M41,
		X * m.M12 + Y * m.M22 + Z * m.M32 + 1 * m.M42,
		X * m.M13 + Y * m.M23 + Z * m.M33 + 1 * m.M43);
}
