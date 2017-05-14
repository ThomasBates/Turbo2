
#pragma once

#include <TurboMatrix4D.h>

struct Vector3D
{
	double X, Y, Z;

	//  Constructors
	Vector3D();
	Vector3D(double x, double y, double z);

	double Length();
	Vector3D Normalize();

	Vector3D operator-() const;

	Vector3D operator+(const Vector3D &v) const;
    Vector3D operator-(const Vector3D &v) const;
    Vector3D operator*(double r) const;
    Vector3D operator/(double r) const;

	Vector3D &operator=(double r);
	Vector3D &operator+=(const Vector3D &v);
	Vector3D &operator-=(const Vector3D &v);
    Vector3D &operator*=(double r);
    Vector3D &operator/=(double r);

    double operator*(const Vector3D &v) const;   //  Dot product
	Vector3D operator%(const Vector3D &v) const;  //  cross product
	
	Vector3D operator*(const TurboMatrix4D &m) const;  //  transform
};

