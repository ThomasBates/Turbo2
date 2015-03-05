
#pragma once

#include "Vector.h"

class Camera
{
public:
	Vector			Right;
	Vector			Up;
	Vector			Back;
	Vector			Position;
	Vector			Target;
	Vector			Velocity;
	Vector			AngularVelocity;

	Camera();

	virtual int		Reset();
	virtual int		GoTo(float x, float y, float z);
	virtual int		GoTo(Vector position);
	virtual int		Move(float x, float y, float z);
	virtual int		Move(Vector velocity);
	virtual int		Rotate(float x, float y, float z);
	virtual int		Rotate(Vector angularVelocity);
};