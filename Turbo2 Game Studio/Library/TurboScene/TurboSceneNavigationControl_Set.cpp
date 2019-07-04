
#include <pch.h>

#include "TurboSceneNavigationControl_Set.h"

using namespace Turbo::Scene;

TurboSceneNavigationControl_Set::TurboSceneNavigationControl_Set(TurboGameControlType type) :
        _type(type)
{
}

void TurboSceneNavigationControl_Set::IsActive(bool isActive)
{
    _isActive = isActive;
}

float TurboSceneNavigationControl_Set::XValue()
{
    return _X;
}

float TurboSceneNavigationControl_Set::YValue()
{
	return _Y;
}

float TurboSceneNavigationControl_Set::ZValue()
{
	return _Z;
}

void TurboSceneNavigationControl_Set::CurrentPoint(float x, float y, float z)
{
    _X = x;
    _Y = y;
	_Z = z;
}
