
#include <math.h>
#include "Vector.h"

Vector::Vector()
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vector::Vector( float x, float y, float z )
{
	X = x;
	Y = y;
	Z = z;
}

float Vector::Length()
{
	return (float)sqrt(X*X + Y*Y + Z*Z);
}

Vector Vector::Normalize()
{
	return *this / Length();
}

Vector Vector::operator-() const
{
	return Vector(-X, -Y, -Z);
}

Vector Vector::operator+(const Vector &v) const
{
	return Vector(X + v.X, Y + v.Y, Z + v.Z);
}

Vector Vector::operator-(const Vector &v) const
{
	return Vector(X - v.X, Y - v.Y, Z - v.Z);
}

Vector Vector::operator*(float r) const
{
	return Vector(X*r, Y*r, Z*r);
}

Vector Vector::operator/(float r) const
{
	if (r != 0)
		return Vector(X/r, Y/r, Z/r);
	return Vector();
}

Vector &Vector::operator=(float r)
{
	*this = Vector(r,r,r);
	return *this;
}

Vector &Vector::operator+=(const Vector &v)
{
	if (this != &v)
		*this = *this + v;
	return *this;
}

Vector &Vector::operator-=(const Vector &v)
{
	if (this != &v)
		*this = *this - v;
	return *this;
}

Vector &Vector::operator*=(float r)
{
	*this = *this * r;
	return *this;
}

Vector &Vector::operator/=(float r)
{
	*this = *this / r;
	return *this;
}

float Vector::operator*(const Vector &v) const	//dot product
{
	return X*v.X + Y*v.Y + Z*v.Z;
}

Vector Vector::operator%(const Vector &v) const	//  cross product
{
	return Vector(Y * v.Z - Z * v.Y,
				  Z * v.X - X * v.Z,
				  X * v.Y - Y * v.X);
}

/*
Vector MakeVector ( float x, float y, float z )
{
	Vector v;
	v.X = x;
	v.Y = y;
	v.Z = z;
	return v;
}

float VectorLength(Vector v)
{
	return (float)sqrt(v.X*v.X + v.Y*v.Y + v.Z*v.Z);
}

Vector Normalize(Vector v)
{
	float length = VectorLength(v);

	if (length == 0.0f)
		return MakeVector(0, 0, 0);

	Vector result;
	result.X = v.X / length;
	result.Y = v.Y / length;
	result.Z = v.Z / length;

	return result;
}

Vector operator+ (Vector left, Vector right)
{
	Vector result;
	result.X = left.X + right.X;
	result.Y = left.Y + right.Y;
	result.Z = left.Z + right.Z;
	return result;
}

Vector operator- (Vector left, Vector right)
{
	Vector result;
	result.X = left.X - right.X;
	result.Y = left.Y - right.Y;
	result.Z = left.Z - right.Z;
	return result;
}


Vector operator* (Vector left, float r)
{
	Vector result;
	result.X = left.X * r;
	result.Y = left.Y * r;
	result.Z = left.Z * r;
	return result;
}

float operator* (Vector left, Vector right)	//dot product
{
	return left.X*right.X + left.Y*right.Y + left.Z*right.Z;
}

Vector CrossProduct(Vector left, Vector right)
{
	Vector result;
	result.X = left.Y * right.Z - left.Z * right.Y;
	result.Y = left.Z * right.X - left.X * right.Z;
	result.Z = left.X * right.Y - left.Y * right.X;

	return result;
}
*/