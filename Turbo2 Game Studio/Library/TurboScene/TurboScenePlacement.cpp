#include <pch.h>

#include <TurboScenePlacement.h>
#include <TurboMatrix4x4.h>

using namespace Turbo::Math;
using namespace Turbo::Scene;

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
	_transform = TurboMatrix4x4();
	_inverse = TurboMatrix4x4();

	_velocity = 0;
	_angularVelocity = 0;
}

void TurboScenePlacement::GoTo(double x, double y, double z)
{
	TurboVector3D delta = TurboVector3D(x, y, z) - Position();

	Move(delta);
}

void TurboScenePlacement::GoTo(TurboVector3D position)
{
	TurboVector3D delta = position - Position();

	Move(delta);
}

void TurboScenePlacement::Move(double x, double y, double z)
{
	Move(TurboVector3D(x, y, z));
}

void TurboScenePlacement::Move(TurboVector3D velocity)
{
	TurboMatrix4x4 translate = TurboMatrix4x4().Translate(velocity);
	TurboMatrix4x4 inverse = TurboMatrix4x4().Translate(-velocity);

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

void TurboScenePlacement::Rotate(TurboVector3D angularVelocity)
{
	Rotate(angularVelocity.X, angularVelocity.Y, angularVelocity.Z);
}

void TurboScenePlacement::RotateX(double degrees)
{
	TurboMatrix4x4 rotate = TurboMatrix4x4().RotateX(degrees);
	TurboMatrix4x4 inverse = TurboMatrix4x4().RotateX(-degrees);

	//  Rotations are pre-multiplied because when a scene object (or player) is at some 
	//	translation, and it rotates, we want it to rotate in place, not around the origin.

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

void TurboScenePlacement::RotateY(double degrees)
{
	TurboMatrix4x4 rotate = TurboMatrix4x4().RotateY(degrees);
	TurboMatrix4x4 inverse = TurboMatrix4x4().RotateY(-degrees);

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

void TurboScenePlacement::RotateZ(double degrees)
{
	TurboMatrix4x4 rotate = TurboMatrix4x4().RotateZ(degrees);
	TurboMatrix4x4 inverse = TurboMatrix4x4().RotateZ(-degrees);

	_transform = rotate * _transform;
	_inverse = _inverse * inverse;
}

#pragma endregion ITurboScenePlacement Methods -------------------------------------------------------------------------
