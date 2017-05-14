
#pragma once

//#include <Vector3D.h>

struct Vector3D;

struct TurboMatrix4D
{
	double M11, M12, M13, M14;
	double M21, M22, M23, M24;
	double M31, M32, M33, M34;
	double M41, M42, M43, M44;

	//  Constructors
	TurboMatrix4D();
	TurboMatrix4D(
		double m11, double m12, double m13, double m14,
		double m21, double m22, double m23, double m24,
		double m31, double m32, double m33, double m34,
		double m41, double m42, double m43, double m44);

	TurboMatrix4D Translate(double x, double y, double z);
	TurboMatrix4D Translate(Vector3D translation);
	TurboMatrix4D Rotate(Vector3D axis, double degrees);
	TurboMatrix4D RotateX(double degrees);
	TurboMatrix4D RotateY(double degrees);
	TurboMatrix4D RotateZ(double degrees);


	//TurboMatrix4D operator+(const TurboMatrix4D &v) const;
 //   TurboMatrix4D operator-(const TurboMatrix4D &v) const;
 //   TurboMatrix4D operator*(double r) const;
 //   TurboMatrix4D operator/(double r) const;

	//TurboMatrix4D &operator=(double r);
	//TurboMatrix4D &operator+=(const TurboMatrix4D &v);
	//TurboMatrix4D &operator-=(const TurboMatrix4D &v);
 //   TurboMatrix4D &operator*=(double r);
 //   TurboMatrix4D &operator/=(double r);

	TurboMatrix4D operator*(const TurboMatrix4D &m) const;

	Vector3D operator*(const Vector3D &v) const;
};

