#pragma once

#include <Vector3D.h>

class ITurboSceneObjectPlacement
{
public:
	//  ITurboSceneObjectPlacement Properties
	virtual Vector3D	Right() = 0;
	virtual void		Right(Vector3D value) = 0;
	virtual Vector3D	Up() = 0;
	virtual void		Up(Vector3D value) = 0;
	virtual Vector3D	Back() = 0;
	virtual void		Back(Vector3D value) = 0;
	virtual Vector3D	Position() = 0;
	virtual void		Position(Vector3D value) = 0;
	virtual Vector3D	Target() = 0;
	virtual void		Target(Vector3D value) = 0;
	virtual Vector3D	Velocity() = 0;
	virtual void		Velocity(Vector3D value) = 0;
	virtual Vector3D	AngularVelocity() = 0;
	virtual void		AngularVelocity(Vector3D value) = 0;

	//  ITurboSceneObjectPlacement Methods
	virtual void	Reset() = 0;
	virtual void	GoTo(float x, float y, float z) = 0;
	virtual void	GoTo(Vector3D position) = 0;
	virtual void	Move(float x, float y, float z) = 0;
	virtual void	Move(Vector3D velocity) = 0;
	virtual void	Rotate(float x, float y, float z) = 0;
	virtual void	Rotate(Vector3D angularVelocity) = 0;
	virtual std::shared_ptr<ITurboSceneObjectPlacement> Clone() = 0;
};

