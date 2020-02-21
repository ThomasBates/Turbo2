#pragma once

#include <pch.h>

#include <ITurboScenePlacement.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboScenePlacement : public ITurboScenePlacement
		{
		public:
			TurboScenePlacement();
			virtual ~TurboScenePlacement() {};

			//  ITurboScenePlacement Properties --------------------------------------------------------------------------------
			virtual TurboMatrix4x4	Transform() { return _transform; }
			virtual TurboMatrix4x4	Inverse() { return _inverse; }

			virtual TurboVector3D	Right() 	{ return TurboVector3D(_transform.M11, _transform.M12, _transform.M13); }
			virtual TurboVector3D	Up() 		{ return TurboVector3D(_transform.M21, _transform.M22, _transform.M23); }
			virtual TurboVector3D	Back() 		{ return TurboVector3D(_transform.M31, _transform.M32, _transform.M33); }
			virtual TurboVector3D	Position() 	{ return TurboVector3D(_transform.M41, _transform.M42, _transform.M43); }
			virtual TurboVector3D	Target() 	{ return Position() - Back(); }

			virtual TurboVector3D	Velocity() { return _velocity; }
			virtual void			Velocity(TurboVector3D value) { _velocity = value; }
			virtual TurboVector3D	AngularVelocity() { return _angularVelocity; }
			virtual void			AngularVelocity(TurboVector3D value) { _angularVelocity = value; }

			//  ITurboScenePlacement Methods -----------------------------------------------------------------------------------
			virtual void Reset();
			virtual void GoTo(double x, double y, double z);
			virtual void GoTo(TurboVector3D position);
			virtual void Move(double x, double y, double z);
			virtual void Move(TurboVector3D velocity);
			virtual void Rotate(double x, double y, double z);
			virtual void Rotate(TurboVector3D angularVelocity);
			virtual void RotateX(double degrees);
			virtual void RotateY(double degrees);
			virtual void RotateZ(double degrees);

		private:
			TurboMatrix4x4	_transform;
			TurboMatrix4x4	_inverse;

			TurboVector3D	_velocity;
			TurboVector3D	_angularVelocity;
		};
	}
}
