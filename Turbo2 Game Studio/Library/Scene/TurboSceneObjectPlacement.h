#pragma once

#include <ITurboSceneObjectPlacement.h>

class TurboSceneObjectPlacement : public ITurboSceneObjectPlacement
{
private:
	Vector3D			_right;
	Vector3D			_up;
	Vector3D			_back;
	Vector3D			_position;
	Vector3D			_target;
	Vector3D			_velocity;
	Vector3D			_angularVelocity;

public:
	TurboSceneObjectPlacement();

	//  ITurboSceneObjectPlacement Properties
	virtual Vector3D	Right() { return _right; }
	virtual void		Right(Vector3D value) { _right = value; }
	virtual Vector3D	Up() { return _up; }
	virtual void		Up(Vector3D value) { _up = value; }
	virtual Vector3D	Back() { return _back; }
	virtual void		Back(Vector3D value) { _back = value; }
	virtual Vector3D	Position() { return _position; }
	virtual void		Position(Vector3D value) { _position = value; }
	virtual Vector3D	Target() { return _target; }
	virtual void		Target(Vector3D value) { _target = value; }
	virtual Vector3D	Velocity() { return _velocity; }
	virtual void		Velocity(Vector3D value) { _velocity = value; }
	virtual Vector3D	AngularVelocity() { return _angularVelocity; }
	virtual void		AngularVelocity(Vector3D value) { _angularVelocity = value; }

	//  ITurboSceneObjectPlacement Methods
	virtual void	Reset();
	virtual void	GoTo(float x, float y, float z);
	virtual void	GoTo(Vector3D position);
	virtual void	Move(float x, float y, float z);
	virtual void	Move(Vector3D velocity);
	virtual void	Rotate(float x, float y, float z);
	virtual void	Rotate(Vector3D angularVelocity);
	virtual std::shared_ptr<ITurboSceneObjectPlacement> Clone();
};

