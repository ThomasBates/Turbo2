
#include <pch.h>
//#include <math.h>
#include <TurboVector2D.h>

Turbo::Math::TurboVector2D::TurboVector2D()
{
	X = 0;
	Y = 0;
}

Turbo::Math::TurboVector2D::TurboVector2D(double x, double y)
{
	X = x;
	Y = y;
}

double Turbo::Math::TurboVector2D::Length()
{
	return (double)sqrt(X*X + Y*Y);
}

Turbo::Math::TurboVector2D Turbo::Math::TurboVector2D::Normalize()
{
	return *this / Length();
}

Turbo::Math::TurboVector2D Turbo::Math::TurboVector2D::operator-() const
{
	return TurboVector2D(-X, -Y);
}

Turbo::Math::TurboVector2D Turbo::Math::TurboVector2D::operator+(const TurboVector2D &v) const
{
	return TurboVector2D(X + v.X, Y + v.Y);
}

Turbo::Math::TurboVector2D Turbo::Math::TurboVector2D::operator-(const TurboVector2D &v) const
{
	return TurboVector2D(X - v.X, Y - v.Y);
}

Turbo::Math::TurboVector2D Turbo::Math::TurboVector2D::operator*(double r) const
{
	return TurboVector2D(X*r, Y*r);
}

Turbo::Math::TurboVector2D Turbo::Math::TurboVector2D::operator/(double r) const
{
	if (r != 0)
		return TurboVector2D(X/r, Y/r);
	return TurboVector2D();
}

Turbo::Math::TurboVector2D &Turbo::Math::TurboVector2D::operator=(double r)
{
	*this = TurboVector2D(r,r);
	return *this;
}

Turbo::Math::TurboVector2D &Turbo::Math::TurboVector2D::operator+=(const TurboVector2D &v)
{
	if (this != &v)
		*this = *this + v;
	return *this;
}

Turbo::Math::TurboVector2D &Turbo::Math::TurboVector2D::operator-=(const TurboVector2D &v)
{
	if (this != &v)
		*this = *this - v;
	return *this;
}

Turbo::Math::TurboVector2D &Turbo::Math::TurboVector2D::operator*=(double r)
{
	*this = *this * r;
	return *this;
}

Turbo::Math::TurboVector2D &Turbo::Math::TurboVector2D::operator/=(double r)
{
	*this = *this / r;
	return *this;
}

double Turbo::Math::TurboVector2D::operator*(const TurboVector2D &v) const	//dot product
{
	return X*v.X + Y*v.Y;
}
