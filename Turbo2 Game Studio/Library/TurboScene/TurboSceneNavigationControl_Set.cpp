
#include <pch.h>

#include "TurboSceneNavigationControl_Set.h"

using namespace Turbo::Core::Debug;
using namespace Turbo::Scene;

TurboSceneNavigationControl_Set::TurboSceneNavigationControl_Set(
        TurboGameControlType type,
        float minX, float maxX,
        float minY, float maxY) :
        _type(type),
        _minX(minX), _maxX(maxX),
        _minY(minY), _maxY(maxY)
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

bool TurboSceneNavigationControl_Set::Contains(float x, float y, float z)
{
    return (_minX <= x && x < _maxX && _minY <= y && y < _maxY);
}

void TurboSceneNavigationControl_Set::CurrentPoint(float x, float y, float z)
{
    _X = x;
    _Y = y;
	_Z = z;
}
