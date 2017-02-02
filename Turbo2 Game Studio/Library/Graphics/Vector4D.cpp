
#include "pch.h"
//#include <math.h>
#include "Vector4D.h"

Vector4D::Vector4D()
{
	X = 0;
	Y = 0;
	Z = 0;
	A = 0;
}

Vector4D::Vector4D( float x, float y, float z, float a )
{
	X = x;
	Y = y;
	Z = z;
	A = a;
}

float Vector4D::Length()
{
	return (float)sqrt(X*X + Y*Y + Z*Z + A*A);
}

Vector4D Vector4D::Normalize()
{
	return *this / Length();
}

Vector4D Vector4D::operator-() const
{
	return Vector4D(-X, -Y, -Z, -A);
}

Vector4D Vector4D::operator+(const Vector4D &v) const
{
	return Vector4D(X + v.X, Y + v.Y, Z + v.Z, A + v.A);
}

Vector4D Vector4D::operator-(const Vector4D &v) const
{
	return Vector4D(X - v.X, Y - v.Y, Z - v.Z, A - v.A);
}

Vector4D Vector4D::operator*(float r) const
{
	return Vector4D(X*r, Y*r, Z*r, A*r);
}

Vector4D Vector4D::operator/(float r) const
{
	if (r != 0)
		return Vector4D(X/r, Y/r, Z/r, A/r);
	return Vector4D();
}

Vector4D &Vector4D::operator=(float r)
{
	*this = Vector4D(r,r,r,r);
	return *this;
}

Vector4D &Vector4D::operator+=(const Vector4D &v)
{
	if (this != &v)
		*this = *this + v;
	return *this;
}

Vector4D &Vector4D::operator-=(const Vector4D &v)
{
	if (this != &v)
		*this = *this - v;
	return *this;
}

Vector4D &Vector4D::operator*=(float r)
{
	*this = *this * r;
	return *this;
}

Vector4D &Vector4D::operator/=(float r)
{
	*this = *this / r;
	return *this;
}

float Vector4D::operator*(const Vector4D &v) const	//dot product
{
	return X*v.X + Y*v.Y + Z*v.Z + A*v.A;
}

//Vector4D Vector4D::operator%(const Vector4D &v) const	//  cross product
//{
//	return Vector4D(Y * v.Z - Z * v.Y,
//					Z * v.X - X * v.Z,
//					X * v.Y - Y * v.X);
//}
