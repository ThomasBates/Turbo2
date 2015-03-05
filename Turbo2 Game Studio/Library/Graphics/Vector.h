
#pragma once

#define PI 3.1415926535897932384626433832795
#define PIby180 ((float)(PI/180.0))


struct Vector
{
	float X, Y, Z;

	//  Constructors
	Vector();
	Vector(float x, float y, float z);

	float Length();
	Vector Normalize();

	Vector operator-() const;

	Vector operator+(const Vector &v) const;
    Vector operator-(const Vector &v) const;
    Vector operator*(float r) const;
    Vector operator/(float r) const;

	Vector &operator=(float r);
	Vector &operator+=(const Vector &v);
	Vector &operator-=(const Vector &v);
    Vector &operator*=(float r);
    Vector &operator/=(float r);

    float operator*(const Vector &v) const;   //  Dot product
	Vector operator%(const Vector &v) const;  //  cross product
};

