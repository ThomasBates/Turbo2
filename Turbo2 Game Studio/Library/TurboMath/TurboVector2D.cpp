
#include "pch.h"
//#include <math.h>
#include "TurboVector2D.h"

TurboVector2D::TurboVector2D()
{
	X = 0;
	Y = 0;
}

TurboVector2D::TurboVector2D(double x, double y)
{
	X = x;
	Y = y;
}

double TurboVector2D::Length()
{
	return (double)sqrt(X*X + Y*Y);
}

TurboVector2D TurboVector2D::Normalize()
{
	return *this / Length();
}

TurboVector2D TurboVector2D::operator-() const
{
	return TurboVector2D(-X, -Y);
}

TurboVector2D TurboVector2D::operator+(const TurboVector2D &v) const
{
	return TurboVector2D(X + v.X, Y + v.Y);
}

TurboVector2D TurboVector2D::operator-(const TurboVector2D &v) const
{
	return TurboVector2D(X - v.X, Y - v.Y);
}

TurboVector2D TurboVector2D::operator*(double r) const
{
	return TurboVector2D(X*r, Y*r);
}

TurboVector2D TurboVector2D::operator/(double r) const
{
	if (r != 0)
		return TurboVector2D(X/r, Y/r);
	return TurboVector2D();
}

TurboVector2D &TurboVector2D::operator=(double r)
{
	*this = TurboVector2D(r,r);
	return *this;
}

TurboVector2D &TurboVector2D::operator+=(const TurboVector2D &v)
{
	if (this != &v)
		*this = *this + v;
	return *this;
}

TurboVector2D &TurboVector2D::operator-=(const TurboVector2D &v)
{
	if (this != &v)
		*this = *this - v;
	return *this;
}

TurboVector2D &TurboVector2D::operator*=(double r)
{
	*this = *this * r;
	return *this;
}

TurboVector2D &TurboVector2D::operator/=(double r)
{
	*this = *this / r;
	return *this;
}

double TurboVector2D::operator*(const TurboVector2D &v) const	//dot product
{
	return X*v.X + Y*v.Y;
}
