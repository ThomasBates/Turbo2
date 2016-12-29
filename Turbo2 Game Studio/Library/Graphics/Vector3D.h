
#pragma once

struct Vector3D
{
	float X, Y, Z;

	//  Constructors
	Vector3D();
	Vector3D(float x, float y, float z);

	float Length();
	Vector3D Normalize();

	Vector3D operator-() const;

	Vector3D operator+(const Vector3D &v) const;
    Vector3D operator-(const Vector3D &v) const;
    Vector3D operator*(float r) const;
    Vector3D operator/(float r) const;

	Vector3D &operator=(float r);
	Vector3D &operator+=(const Vector3D &v);
	Vector3D &operator-=(const Vector3D &v);
    Vector3D &operator*=(float r);
    Vector3D &operator/=(float r);

    float operator*(const Vector3D &v) const;   //  Dot product
	Vector3D operator%(const Vector3D &v) const;  //  cross product
};

