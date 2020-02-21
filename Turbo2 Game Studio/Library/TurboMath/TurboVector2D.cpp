
#include <pch.h>

#include <TurboVector2D.h>

using namespace Turbo::Math;

//  Constructors -------------------------------------------------------------------------------------------------------

TurboVector2D::TurboVector2D()
{
	X = 0;
	Y = 0;
}

TurboVector2D::TurboVector2D(float x, float y)
{
	X = x;
	Y = y;
}

//	TurboVector3D Methods ----------------------------------------------------------------------------------------------

float TurboVector2D::Length()
{
	return (float)sqrt(X*X + Y*Y);
}

TurboVector2D TurboVector2D::Normalize()
{
	return *this / Length();
}

//	TurboVector3D Operators --------------------------------------------------------------------------------------------

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

TurboVector2D TurboVector2D::operator*(float r) const
{
	return TurboVector2D(X*r, Y*r);
}

TurboVector2D TurboVector2D::operator/(float r) const
{
	if (r != 0)
		return TurboVector2D(X/r, Y/r);
	return TurboVector2D();
}

TurboVector2D &TurboVector2D::operator=(float r)
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

TurboVector2D &TurboVector2D::operator*=(float r)
{
	*this = *this * r;
	return *this;
}

TurboVector2D &TurboVector2D::operator/=(float r)
{
	*this = *this / r;
	return *this;
}

float TurboVector2D::operator*(const TurboVector2D &v) const	//dot product
{
	return X*v.X + Y*v.Y;
}
