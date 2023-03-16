
#include <pch.h>

#include <TurboVector3D.h>

using namespace Turbo::Math;

//  Constructors -------------------------------------------------------------------------------------------------------

TurboVector3D::TurboVector3D()
{
	X = 0;
	Y = 0;
	Z = 0;
}

TurboVector3D::TurboVector3D( float x, float y, float z )
{
	X = x;
	Y = y;
	Z = z;
}

//	TurboVector3D Methods ----------------------------------------------------------------------------------------------

float TurboVector3D::Length() const
{
	return sqrt(X*X + Y*Y + Z*Z);
}

TurboVector3D TurboVector3D::Normalize() const
{
	return *this / Length();
}

float TurboVector3D::LengthSquared() const
{
	return X*X + Y*Y + Z*Z;
}

//	TurboVector3D Operators --------------------------------------------------------------------------------------------

TurboVector3D TurboVector3D::operator-() const
{
	return TurboVector3D(-X, -Y, -Z);
}

TurboVector3D TurboVector3D::operator+(const TurboVector3D &v) const
{
	return TurboVector3D(X + v.X, Y + v.Y, Z + v.Z);
}

TurboVector3D TurboVector3D::operator-(const TurboVector3D &v) const
{
	return TurboVector3D(X - v.X, Y - v.Y, Z - v.Z);
}

TurboVector3D TurboVector3D::operator*(float r) const
{
	return TurboVector3D(X*r, Y*r, Z*r);
}

TurboVector3D TurboVector3D::operator/(float r) const
{
	if (r != 0)
		return TurboVector3D(X/r, Y/r, Z/r);
	return TurboVector3D();
}

TurboVector3D &TurboVector3D::operator=(float r)
{
	*this = TurboVector3D(r,r,r);
	return *this;
}

TurboVector3D &TurboVector3D::operator+=(const TurboVector3D &v)
{
	if (this != &v)
		*this = *this + v;
	return *this;
}

TurboVector3D &TurboVector3D::operator-=(const TurboVector3D &v)
{
	if (this != &v)
		*this = *this - v;
	return *this;
}

TurboVector3D &TurboVector3D::operator*=(float r)
{
	*this = *this * r;
	return *this;
}

TurboVector3D &TurboVector3D::operator/=(float r)
{
	*this = *this / r;
	return *this;
}

float TurboVector3D::operator*(const TurboVector3D &v) const	//dot product
{
	return X*v.X + Y*v.Y + Z*v.Z;
}

TurboVector3D TurboVector3D::operator%(const TurboVector3D &v) const	//  cross product
{
	return TurboVector3D(
		Y * v.Z - Z * v.Y,
		Z * v.X - X * v.Z,
		X * v.Y - Y * v.X);
}

TurboVector3D TurboVector3D::operator*(const TurboMatrix4x4 & m) const
{
	return TurboVector3D(
		X * m.M11 + Y * m.M21 + Z * m.M31 + 1 * m.M41,
		X * m.M12 + Y * m.M22 + Z * m.M32 + 1 * m.M42,
		X * m.M13 + Y * m.M23 + Z * m.M33 + 1 * m.M43);
}
