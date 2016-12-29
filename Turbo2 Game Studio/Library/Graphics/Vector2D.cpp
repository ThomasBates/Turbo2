
#include "pch.h"
//#include <math.h>
#include "Vector2D.h"

Vector2D::Vector2D()
{
	X = 0;
	Y = 0;
}

Vector2D::Vector2D(float x, float y)
{
	X = x;
	Y = y;
}

float Vector2D::Length()
{
	return (float)sqrt(X*X + Y*Y);
}

Vector2D Vector2D::Normalize()
{
	return *this / Length();
}

Vector2D Vector2D::operator-() const
{
	return Vector2D(-X, -Y);
}

Vector2D Vector2D::operator+(const Vector2D &v) const
{
	return Vector2D(X + v.X, Y + v.Y);
}

Vector2D Vector2D::operator-(const Vector2D &v) const
{
	return Vector2D(X - v.X, Y - v.Y);
}

Vector2D Vector2D::operator*(float r) const
{
	return Vector2D(X*r, Y*r);
}

Vector2D Vector2D::operator/(float r) const
{
	if (r != 0)
		return Vector2D(X/r, Y/r);
	return Vector2D();
}

Vector2D &Vector2D::operator=(float r)
{
	*this = Vector2D(r,r);
	return *this;
}

Vector2D &Vector2D::operator+=(const Vector2D &v)
{
	if (this != &v)
		*this = *this + v;
	return *this;
}

Vector2D &Vector2D::operator-=(const Vector2D &v)
{
	if (this != &v)
		*this = *this - v;
	return *this;
}

Vector2D &Vector2D::operator*=(float r)
{
	*this = *this * r;
	return *this;
}

Vector2D &Vector2D::operator/=(float r)
{
	*this = *this / r;
	return *this;
}

float Vector2D::operator*(const Vector2D &v) const	//dot product
{
	return X*v.X + Y*v.Y;
}
