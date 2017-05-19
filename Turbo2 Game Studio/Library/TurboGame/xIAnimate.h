#pragma once

#include <Vector3D.h>

class IAnimate
{
	virtual Vector3D Evaluate(float t) = 0;

	virtual int		IsActive(float t) = 0;
	virtual int		IsFinished(float t) = 0;

	virtual float	GetStart() = 0;
	virtual void	SetStart(float start) = 0;
	virtual float	GetDuration() = 0;
	virtual void	SetDuration(float duration) = 0;
	virtual int		GetIsContinuous() = 0;
	virtual void	SetIsContinuous(int isContinuous) = 0;
};
