
#include <math.h>
#include "Camera.h"

Camera::Camera()
{
	Reset();
}

int Camera::Reset()
{
	Right    = Vector(1,0,0);
	Up       = Vector(0,1,0);
	Back     = Vector(0,0,1);
	Position = 0;
	Velocity = 0;
	AngularVelocity = 0;
	Target   = Position - Back;
	return 1;
}

int Camera::GoTo(float x, float y, float z)
{
	Position = Vector(x,y,z);

	Target = Position - Back;

	return 1;
}

int Camera::GoTo(Vector position)
{
	Position = position;

	Target = Position - Back;

	return 1;
}

int Camera::Move(float x, float y, float z)
{
	Position += Right * x + Up * y + Back * z;

	Target = Position - Back;

	return 1;
}

int Camera::Move(Vector velocity)
{
	Position += velocity;

	Target = Position - Back;

	return 1;
}

int Camera::Rotate(float x, float y, float z)
{
	if (x != 0)
	{
		Up = (Up * cos(-x * PIby180) +
			 Back * sin(-x * PIby180)).Normalize();
		Back = Right % Up;  //  CrossProduct
	}

	if (y != 0)
	{
		Back = (Back * cos(y * PIby180)
			  + Right * sin(y * PIby180)).Normalize();
		Right = Up % Back;  //  CrossProduct
	}

	if (z != 0)
	{
		Right = (Right * cos(z * PIby180)
				   + Up * sin(z * PIby180)).Normalize();
		Up = Back % Right;  //  CrossProduct
	}

	Target = Position - Back;

	return 1;
}

int Camera::Rotate(Vector angularVelocity)
{
	return Rotate(angularVelocity.X, angularVelocity.Y, angularVelocity.Z);
}
