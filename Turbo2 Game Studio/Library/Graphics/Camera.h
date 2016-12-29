
#pragma once

#include "Vector3D.h"

class Camera
{
public:
	Vector3D			Right;
	Vector3D			Up;
	Vector3D			Back;
	Vector3D			Position;
	Vector3D			Target;
	Vector3D			Velocity;
	Vector3D			AngularVelocity;

	Camera();

	virtual int		Reset();
	virtual int		GoTo(float x, float y, float z);
	virtual int		GoTo(Vector3D position);
	virtual int		Move(float x, float y, float z);
	virtual int		Move(Vector3D velocity);
	virtual int		Rotate(float x, float y, float z);
	virtual int		Rotate(Vector3D angularVelocity);
};