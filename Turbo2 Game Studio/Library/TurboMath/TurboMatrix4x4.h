
#pragma once

//#include <TurboVector3D.h>

//#include "TurboVector3D.h"

#define PI 3.1415926535897932384626433832795
#define PIby180 ((float)(PI/180.0))

namespace Turbo
{
	namespace Math
	{
		//	Get around circular includes.
		struct TurboVector3D;

		struct TurboMatrix4x4
		{
			float M11, M12, M13, M14;
			float M21, M22, M23, M24;
			float M31, M32, M33, M34;
			float M41, M42, M43, M44;

			//  Constructors
			TurboMatrix4x4();
			TurboMatrix4x4(
				float m11, float m12, float m13, float m14,
				float m21, float m22, float m23, float m24,
				float m31, float m32, float m33, float m34,
				float m41, float m42, float m43, float m44);

			float *Ptr() {return &M11;}
            TurboMatrix4x4 Transpose();
            TurboMatrix4x4 Translate(float x, float y, float z);
			TurboMatrix4x4 Translate(TurboVector3D translation);
			TurboMatrix4x4 Rotate(TurboVector3D axis, float degrees);
			TurboMatrix4x4 RotateX(float degrees);
			TurboMatrix4x4 RotateY(float degrees);
			TurboMatrix4x4 RotateZ(float degrees);


			//TurboMatrix4x4 operator+(const TurboMatrix4x4 &v) const;
		 //   TurboMatrix4x4 operator-(const TurboMatrix4x4 &v) const;
		 //   TurboMatrix4x4 operator*(float r) const;
		 //   TurboMatrix4x4 operator/(float r) const;

			//TurboMatrix4x4 &operator=(float r);
			//TurboMatrix4x4 &operator+=(const TurboMatrix4x4 &v);
			//TurboMatrix4x4 &operator-=(const TurboMatrix4x4 &v);
		 //   TurboMatrix4x4 &operator*=(float r);
		 //   TurboMatrix4x4 &operator/=(float r);

			TurboMatrix4x4 operator*(const TurboMatrix4x4 &m) const;

			TurboVector3D operator*(const TurboVector3D &v) const;
		};
	}
}
