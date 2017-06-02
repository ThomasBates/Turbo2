#include <pch.h>

#include <TurboScenePlacement.h>

#define PI 3.1415926535897932384626433832795
#define PIby180 ((double)(PI/180.0))


#pragma region Constructors --------------------------------------------------------------------------------------------

Turbo::Scene::TurboScenePlacement::TurboScenePlacement()
{
	Reset();
}

#pragma endregion
#pragma region ITurboScenePlacement Properties -------------------------------------------------------------------------

#pragma endregion ITurboScenePlacement Properties ----------------------------------------------------------------------
#pragma region ITurboScenePlacement Methods ----------------------------------------------------------------------------

void Turbo::Scene::TurboScenePlacement::Reset()
{
	_position = 0;
	_transform = TurboMatrix4x4();
	_inverse = TurboMatrix4x4();

	//_right = TurboVector3D(1, 0, 0);
	//_up = TurboVector3D(0, 1, 0);
	//_back = TurboVector3D(0, 0, 1);
	//_target = _position - _back;

	_velocity = 0;
	_angularVelocity = 0;
}

void Turbo::Scene::TurboScenePlacement::GoTo(double x, double y, double z)
{
	TurboVector3D delta = TurboVector3D(x, y, z) - Position();

	Move(delta);
}

void Turbo::Scene::TurboScenePlacement::GoTo(TurboVector3D position)
{
	TurboVector3D delta = position - Position();

	Move(delta);
}

void Turbo::Scene::TurboScenePlacement::Move(double x, double y, double z)
{
	Move(TurboVector3D(x, y, z));
}

void Turbo::Scene::TurboScenePlacement::Move(TurboVector3D velocity)
{
	TurboMatrix4x4 translate = TurboMatrix4x4().Translate(velocity);
	TurboMatrix4x4 inverse = TurboMatrix4x4().Translate(-velocity);

	_transform = _transform * translate;
	_inverse = inverse * _inverse;
}

void Turbo::Scene::TurboScenePlacement::Rotate(double x, double y, double z)
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

void Turbo::Scene::TurboScenePlacement::Rotate(TurboVector3D angularVelocity)
{
	Rotate(angularVelocity.X, angularVelocity.Y, angularVelocity.Z);
}

void Turbo::Scene::TurboScenePlacement::RotateX(double degrees)
{
	TurboMatrix4x4 rotate = TurboMatrix4x4().RotateX(degrees);
	TurboMatrix4x4 inverse = TurboMatrix4x4().RotateX(-degrees);

	//  Rotations are pre-multiplied because when a scene object (or player) is at some 
	//	translation, and it rotates, we want it to rotate in place, not around the origin.

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

void Turbo::Scene::TurboScenePlacement::RotateY(double degrees)
{
	TurboMatrix4x4 rotate = TurboMatrix4x4().RotateY(degrees);
	TurboMatrix4x4 inverse = TurboMatrix4x4().RotateY(-degrees);

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

void Turbo::Scene::TurboScenePlacement::RotateZ(double degrees)
{
	TurboMatrix4x4 rotate = TurboMatrix4x4().RotateZ(degrees);
	TurboMatrix4x4 inverse = TurboMatrix4x4().RotateZ(-degrees);

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

//void Turbo::Scene::TurboScenePlacement::Transform(TurboMatrix4x4 transform)
//{
//	_transform = _transform * transform;
//	_inverse = transform * _inverse;
//}

//std::shared_ptr<ITurboScenePlacement> Turbo::Scene::TurboScenePlacement::Clone()
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
