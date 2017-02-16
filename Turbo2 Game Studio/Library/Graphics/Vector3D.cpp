
#include "pch.h"
//#include <math.h>
#include "Vector3D.h"

Vector3D::Vector3D()
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vector3D::Vector3D( double x, double y, double z )
{
	X = x;
	Y = y;
	Z = z;
}

double Vector3D::Length()
{
	return (double)sqrt(X*X + Y*Y + Z*Z);
}

Vector3D Vector3D::Normalize()
{
	return *this / Length();
}

Vector3D Vector3D::operator-() const
{
	return Vector3D(-X, -Y, -Z);
}

Vector3D Vector3D::operator+(const Vector3D &v) const
{
	return Vector3D(X + v.X, Y + v.Y, Z + v.Z);
}

Vector3D Vector3D::operator-(const Vector3D &v) const
{
	return Vector3D(X - v.X, Y - v.Y, Z - v.Z);
}

Vector3D Vector3D::operator*(double r) const
{
	return Vector3D(X*r, Y*r, Z*r);
}

Vector3D Vector3D::operator/(double r) const
{
	if (r != 0)
		return Vector3D(X/r, Y/r, Z/r);
	return Vector3D();
}

Vector3D &Vector3D::operator=(double r)
{
	*this = Vector3D(r,r,r);
	return *this;
}

Vector3D &Vector3D::operator+=(const Vector3D &v)
{
	if (this != &v)
		*this = *this + v;
	return *this;
}

Vector3D &Vector3D::operator-=(const Vector3D &v)
{
	if (this != &v)
		*this = *this - v;
	return *this;
}

Vector3D &Vector3D::operator*=(double r)
{
	*this = *this * r;
	return *this;
}

Vector3D &Vector3D::operator/=(double r)
{
	*this = *this / r;
	return *this;
}

double Vector3D::operator*(const Vector3D &v) const	//dot product
{
	return X*v.X + Y*v.Y + Z*v.Z;
}

Vector3D Vector3D::operator%(const Vector3D &v) const	//  cross product
{
	return Vector3D(Y * v.Z - Z * v.Y,
					Z * v.X - X * v.Z,
					X * v.Y - Y * v.X);
}
