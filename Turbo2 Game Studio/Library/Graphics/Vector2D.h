
#pragma once

struct Vector2D
{
	double X, Y;

	//  Constructors
	Vector2D();
	Vector2D(double x, double y);

	double Length();
	Vector2D Normalize();

	Vector2D operator-() const;

	Vector2D operator+(const Vector2D &v) const;
    Vector2D operator-(const Vector2D &v) const;
    Vector2D operator*(double r) const;
    Vector2D operator/(double r) const;

	Vector2D &operator=(double r);
	Vector2D &operator+=(const Vector2D &v);
	Vector2D &operator-=(const Vector2D &v);
    Vector2D &operator*=(double r);
    Vector2D &operator/=(double r);

    double operator*(const Vector2D &v) const;   //  Dot product
};

