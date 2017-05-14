#include "pch.h"

#include <TurboScenePlacement.h>

#define PI 3.1415926535897932384626433832795
#define PIby180 ((double)(PI/180.0))


#pragma region Constructors --------------------------------------------------------------------------------------------

TurboScenePlacement::TurboScenePlacement()
{
	Reset();
}

#pragma endregion
#pragma region ITurboScenePlacement Properties -------------------------------------------------------------------------

#pragma endregion ITurboScenePlacement Properties ----------------------------------------------------------------------
#pragma region ITurboScenePlacement Methods ----------------------------------------------------------------------------

void TurboScenePlacement::Reset()
{
	_position = 0;
	_transform = TurboMatrix4D();
	_inverse = TurboMatrix4D();

	//_right = Vector3D(1, 0, 0);
	//_up = Vector3D(0, 1, 0);
	//_back = Vector3D(0, 0, 1);
	//_target = _position - _back;

	_velocity = 0;
	_angularVelocity = 0;
}

void TurboScenePlacement::GoTo(double x, double y, double z)
{
	Vector3D delta = Vector3D(x, y, z) - Position();

	Move(delta);
}

void TurboScenePlacement::GoTo(Vector3D position)
{
	Vector3D delta = position - Position();

	Move(delta);
}

void TurboScenePlacement::Move(double x, double y, double z)
{
	Move(Vector3D(x, y, z));
}

void TurboScenePlacement::Move(Vector3D velocity)
{
	TurboMatrix4D translate = TurboMatrix4D().Translate(velocity);
	TurboMatrix4D inverse = TurboMatrix4D().Translate(-velocity);

	_transform = _transform * translate;
	_inverse = inverse * _inverse;
}

void TurboScenePlacement::Rotate(double x, double y, double z)
{
	if (x != 0)
	{
		RotateX(x);
	}

	if (y != 0)
	{
		RotateY(y);
	}

	if (z != 0)
	{
		RotateZ(z);
	}
}

void TurboScenePlacement::Rotate(Vector3D angularVelocity)
{
	Rotate(angularVelocity.X, angularVelocity.Y, angularVelocity.Z);
}

void TurboScenePlacement::RotateX(double degrees)
{
	TurboMatrix4D rotate = TurboMatrix4D().RotateX(degrees);
	TurboMatrix4D inverse = TurboMatrix4D().RotateX(-degrees);

	//  Rotations are pre-multiplied because when a scene object (or player) is at some 
	//	translation, and it rotates, we want it to rotate in place, not around the origin.

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

void TurboScenePlacement::RotateY(double degrees)
{
	TurboMatrix4D rotate = TurboMatrix4D().RotateY(degrees);
	TurboMatrix4D inverse = TurboMatrix4D().RotateY(-degrees);

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

void TurboScenePlacement::RotateZ(double degrees)
{
	TurboMatrix4D rotate = TurboMatrix4D().RotateZ(degrees);
	TurboMatrix4D inverse = TurboMatrix4D().RotateZ(-degrees);

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

//void TurboScenePlacement::Transform(TurboMatrix4D transform)
//{
//	_transform = _transform * transform;
//	_inverse = transform * _inverse;
//}

//std::shared_ptr<ITurboScenePlacement> TurboScenePlacement::Clone()
//{
//	std::shared_ptr<ITurboScenePlacement> clone =
//		std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
//
//	clone->Right(_right);
//	clone->Up(_up);
//	clone->Back(_back);
//	clone->Position(_position);
//	clone->Target(_target);
//	clone->Velocity(_velocity);
//	clone->AngularVelocity(_angularVelocity);
//
//	return clone;
//}

#pragma endregion ITurboScenePlacement Methods -------------------------------------------------------------------------
