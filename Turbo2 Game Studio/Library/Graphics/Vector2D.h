
#pragma once

struct Vector2D
{
	float X, Y;

	//  Constructors
	Vector2D();
	Vector2D(float x, float y);

	float Length();
	Vector2D Normalize();

	Vector2D operator-() const;

	Vector2D operator+(const Vector2D &v) const;
    Vector2D operator-(const Vector2D &v) const;
    Vector2D operator*(float r) const;
    Vector2D operator/(float r) const;

	Vector2D &operator=(float r);
	Vector2D &operator+=(const Vector2D &v);
	Vector2D &operator-=(const Vector2D &v);
    Vector2D &operator*=(float r);
    Vector2D &operator/=(float r);

    float operator*(const Vector2D &v) const;   //  Dot product
};

