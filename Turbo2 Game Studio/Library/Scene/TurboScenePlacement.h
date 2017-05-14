#pragma once

#include <ITurboScenePlacement.h>
#include <TurboMatrix4D.h>

class TurboScenePlacement : public ITurboScenePlacement
{
private:
	TurboMatrix4D		_transform;
	TurboMatrix4D		_inverse;

	Vector3D			_right;
	Vector3D			_up;
	Vector3D			_back;
	Vector3D			_position;
	Vector3D			_target;

	Vector3D			_velocity;
	Vector3D			_angularVelocity;

public:
	TurboScenePlacement();

	//  ITurboScenePlacement Properties  -------------------------------------------------------------------------
	virtual TurboMatrix4D	Transform() { return _transform; }
	virtual TurboMatrix4D	Inverse() { return _inverse; }

	virtual Vector3D		Right() { return Vector3D(_transform.M11, _transform.M12, _transform.M13); }
	//virtual Vector3D		Right() { return _right; }
	virtual Vector3D		Up() { return Vector3D(_transform.M21, _transform.M22, _transform.M23); }
	//virtual Vector3D		Up() { return _up; }
	virtual Vector3D		Back() { return Vector3D(_transform.M31, _transform.M32, _transform.M33); }
	//virtual Vector3D		Back() { return _back; }
	virtual Vector3D		Position() { return Vector3D(_transform.M41, _transform.M42, _transform.M43); }
	virtual Vector3D		Target() { return Position() - Back(); }
	//virtual Vector3D		Target() { return _target; }

	virtual Vector3D		Velocity() { return _velocity; }
	virtual void			Velocity(Vector3D value) { _velocity = value; }
	virtual Vector3D		AngularVelocity() { return _angularVelocity; }
	virtual void			AngularVelocity(Vector3D value) { _angularVelocity = value; }

	//  ITurboScenePlacement Methods  -------------------------------------------------------------------------------
	virtual void	Reset();
	virtual void	GoTo(double x, double y, double z);
	virtual void	GoTo(Vector3D position);
	virtual void	Move(double x, double y, double z);
	virtual void	Move(Vector3D velocity);
	virtual void	Rotate(double x, double y, double z);
	virtual void	Rotate(Vector3D angularVelocity);
	virtual void	RotateX(double x);
	virtual void	RotateY(double y);
	virtual void	RotateZ(double z);
	//virtual void	Transform(TurboMatrix4D transform);
	//virtual std::shared_ptr<ITurboScenePlacement> Clone();
};

