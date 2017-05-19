
#pragma once

//#include <TurboVector3D.h>

struct TurboVector3D;

struct TurboMatrix4x4
{
	double M11, M12, M13, M14;
	double M21, M22, M23, M24;
	double M31, M32, M33, M34;
	double M41, M42, M43, M44;

	//  Constructors
	TurboMatrix4x4();
	TurboMatrix4x4(
		double m11, double m12, double m13, double m14,
		double m21, double m22, double m23, double m24,
		double m31, double m32, double m33, double m34,
		double m41, double m42, double m43, double m44);

	TurboMatrix4x4 Translate(double x, double y, double z);
	TurboMatrix4x4 Translate(TurboVector3D translation);
	TurboMatrix4x4 Rotate(TurboVector3D axis, double degrees);
	TurboMatrix4x4 RotateX(double degrees);
	TurboMatrix4x4 RotateY(double degrees);
	TurboMatrix4x4 RotateZ(double degrees);


	//TurboMatrix4x4 operator+(const TurboMatrix4x4 &v) const;
 //   TurboMatrix4x4 operator-(const TurboMatrix4x4 &v) const;
 //   TurboMatrix4x4 operator*(double r) const;
 //   TurboMatrix4x4 operator/(double r) const;

	//TurboMatrix4x4 &operator=(double r);
	//TurboMatrix4x4 &operator+=(const TurboMatrix4x4 &v);
	//TurboMatrix4x4 &operator-=(const TurboMatrix4x4 &v);
 //   TurboMatrix4x4 &operator*=(double r);
 //   TurboMatrix4x4 &operator/=(double r);

	TurboMatrix4x4 operator*(const TurboMatrix4x4 &m) const;

	TurboVector3D operator*(const TurboVector3D &v) const;
};

