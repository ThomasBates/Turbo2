#include "pch.h"

#include <TurboSceneObjectPlacement.h>

#define PI 3.1415926535897932384626433832795
#define PIby180 ((double)(PI/180.0))


TurboSceneObjectPlacement::TurboSceneObjectPlacement()
{
	Reset();
}

void TurboSceneObjectPlacement::Reset()
{
	_right = Vector3D(1, 0, 0);
	_up = Vector3D(0, 1, 0);
	_back = Vector3D(0, 0, 1);
	_position = 0;
	_velocity = 0;
	_angularVelocity = 0;
	_target = _position - _back;
}

void TurboSceneObjectPlacement::GoTo(double x, double y, double z)
{
	_position = Vector3D(x, y, z);

	_target = _position - _back;
}

void TurboSceneObjectPlacement::GoTo(Vector3D position)
{
	_position = position;

	_target = _position - _back;
}

void TurboSceneObjectPlacement::Move(double x, double y, double z)
{
	_position += _right * x + _up * y + _back * z;

	_target = _position - _back;
}

void TurboSceneObjectPlacement::Move(Vector3D velocity)
{
	_position += velocity;

	_target = _position - _back;
}

void TurboSceneObjectPlacement::Rotate(double x, double y, double z)
{
	if (x != 0)
	{
		_up = (_up * cos(-x * PIby180) +
			_back * sin(-x * PIby180)).Normalize();
		_back = _right % _up;  //  CrossProduct
	}

	if (y != 0)
	{
		_back = (_back * cos(y * PIby180)
			+ _right * sin(y * PIby180)).Normalize();
		_right = _up % _back;  //  CrossProduct
	}

	if (z != 0)
	{
		_right = (_right * cos(z * PIby180)
			+ _up * sin(z * PIby180)).Normalize();
		_up = _back % _right;  //  CrossProduct
	}

	_target = _position - _back;
}

void TurboSceneObjectPlacement::Rotate(Vector3D angularVelocity)
{
	Rotate(angularVelocity.X, angularVelocity.Y, angularVelocity.Z);
}

std::shared_ptr<ITurboSceneObjectPlacement> TurboSceneObjectPlacement::Clone()
{
	std::shared_ptr<ITurboSceneObjectPlacement> clone = std::shared_ptr<ITurboSceneObjectPlacement>(new TurboSceneObjectPlacement());

	clone->Right(_right);
	clone->Up(_up);
	clone->Back(_back);
	clone->Position(_position);
	clone->Target(_target);
	clone->Velocity(_velocity);
	clone->AngularVelocity(_angularVelocity);

	return clone;
}
