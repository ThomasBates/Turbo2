
#include <pch.h>
//#include <math.h>
#include <TurboVector3D.h>
#include <TurboMatrix4x4.h>
#include "TurboMatrix4x4.h"


using namespace Turbo::Math;

TurboMatrix4x4::TurboMatrix4x4()
{
	//	Initialize to identity matrix by default
	M11 = 1; M12 = 0; M13 = 0; M14 = 0;
	M21 = 0; M22 = 1; M23 = 0; M24 = 0;
	M31 = 0; M32 = 0; M33 = 1; M34 = 0;
	M41 = 0; M42 = 0; M43 = 0; M44 = 1;
}

TurboMatrix4x4::TurboMatrix4x4(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	M11 = m11; M12 = m12; M13 = m13; M14 = m14;
	M21 = m21; M22 = m22; M23 = m23; M24 = m24;
	M31 = m31; M32 = m32; M33 = m33; M34 = m34;
	M41 = m41; M42 = m42; M43 = m43; M44 = m44;
}

TurboMatrix4x4 TurboMatrix4x4::Transpose()
{
    TurboMatrix4x4 transpose = TurboMatrix4x4(
            M11, M21, M31, M41,
            M12, M22, M32, M42,
            M13, M23, M33, M43,
            M14, M24, M34, M44);
    return transpose;
}

TurboMatrix4x4 TurboMatrix4x4::Translate(float x, float y, float z)
{
	TurboMatrix4x4 translate = TurboMatrix4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1);

	return *this * translate;
}

TurboMatrix4x4 TurboMatrix4x4::Translate(TurboVector3D translation)
{
	return Translate(translation.X, translation.Y, translation.Z);
}

TurboMatrix4x4 TurboMatrix4x4::Scale(float x, float y, float z)
{
	TurboMatrix4x4 scale = TurboMatrix4x4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);

	return *this * scale;
}

TurboMatrix4x4 TurboMatrix4x4::Scale(TurboVector3D scale)
{
	return Translate(scale.X, scale.Y, scale.Z);
}

TurboMatrix4x4 TurboMatrix4x4::Rotate(TurboVector3D axis, float degrees)
{
	return TurboMatrix4x4();
}

TurboMatrix4x4 TurboMatrix4x4::RotateX(float degrees)
{
	float c = cos(degrees * PIby180);
	float s = sin(degrees * PIby180);

	TurboMatrix4x4 rotate = TurboMatrix4x4(
		1, 0, 0, 0,
		0, c, s, 0,
		0,-s, c, 0,
		0, 0, 0, 1);

	return *this * rotate;
}

TurboMatrix4x4 TurboMatrix4x4::RotateY(float degrees)
{
	float c = cos(degrees * PIby180);
	float s = sin(degrees * PIby180);

	TurboMatrix4x4 rotate = TurboMatrix4x4(
		c, 0,-s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1);

	return *this * rotate;
}

TurboMatrix4x4 TurboMatrix4x4::RotateZ(float degrees)
{
	float c = cos(degrees * PIby180);
	float s = sin(degrees * PIby180);

	TurboMatrix4x4 rotate = TurboMatrix4x4(
		c, s, 0, 0,
		-s,c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	return *this * rotate;
}

TurboMatrix4x4 TurboMatrix4x4::operator*(const TurboMatrix4x4 &m) const	//  cross product
{
	TurboMatrix4x4 result = TurboMatrix4x4();

	result.M11 = M11 * m.M11 + M12 * m.M21 + M13 * m.M31 + M14 * m.M41;
	result.M12 = M11 * m.M12 + M12 * m.M22 + M13 * m.M32 + M14 * m.M42;
	result.M13 = M11 * m.M13 + M12 * m.M23 + M13 * m.M33 + M14 * m.M43;
	result.M14 = M11 * m.M14 + M12 * m.M24 + M13 * m.M34 + M14 * m.M44;

	result.M21 = M21 * m.M11 + M22 * m.M21 + M23 * m.M31 + M24 * m.M41;
	result.M22 = M21 * m.M12 + M22 * m.M22 + M23 * m.M32 + M24 * m.M42;
	result.M23 = M21 * m.M13 + M22 * m.M23 + M23 * m.M33 + M24 * m.M43;
	result.M24 = M21 * m.M14 + M22 * m.M24 + M23 * m.M34 + M24 * m.M44;

	result.M31 = M31 * m.M11 + M32 * m.M21 + M33 * m.M31 + M34 * m.M41;
	result.M32 = M31 * m.M12 + M32 * m.M22 + M33 * m.M32 + M34 * m.M42;
	result.M33 = M31 * m.M13 + M32 * m.M23 + M33 * m.M33 + M34 * m.M43;
	result.M34 = M31 * m.M14 + M32 * m.M24 + M33 * m.M34 + M34 * m.M44;

	result.M41 = M41 * m.M11 + M42 * m.M21 + M43 * m.M31 + M44 * m.M41;
	result.M42 = M41 * m.M12 + M42 * m.M22 + M43 * m.M32 + M44 * m.M42;
	result.M43 = M41 * m.M13 + M42 * m.M23 + M43 * m.M33 + M44 * m.M43;
	result.M44 = M41 * m.M14 + M42 * m.M24 + M43 * m.M34 + M44 * m.M44;

	return result;
}

TurboVector3D TurboMatrix4x4::operator*(const TurboVector3D & v) const
{
	return TurboVector3D();
}
