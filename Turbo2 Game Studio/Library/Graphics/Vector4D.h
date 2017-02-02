
#pragma once

struct Vector4D
{
	float X, Y, Z, A;

	//  Constructors
	Vector4D();
	Vector4D(float x, float y, float z, float a);

	float Length();
	Vector4D Normalize();

	Vector4D operator-() const;

	Vector4D operator+(const Vector4D &v) const;
    Vector4D operator-(const Vector4D &v) const;
    Vector4D operator*(float r) const;
    Vector4D operator/(float r) const;

	Vector4D &operator=(float r);
	Vector4D &operator+=(const Vector4D &v);
	Vector4D &operator-=(const Vector4D &v);
    Vector4D &operator*=(float r);
    Vector4D &operator/=(float r);

    float operator*(const Vector4D &v) const;   //  Dot product
	//Vector4D operator%(const Vector4D &v) const;  //  cross product
};

