
#include <pch.h>

#include "TurboSceneNavigationControl_Base.h"

using namespace Turbo::Scene;

TurboSceneNavigationControl_Base::TurboSceneNavigationControl_Base(
        TurboGameControlType type,
        float minX, float maxX,
        float minY, float maxY,
        float scale) :
        _type(type),
        _minX(minX), _maxX(maxX),
        _minY(minY), _maxY(maxY),
        _scale(scale)
{
}

void TurboSceneNavigationControl_Base::IsActive(bool isActive)
{
    if (_isActive != isActive)
    {
        _isActive = isActive;
        _firstPoint = isActive;
    }
}

float TurboSceneNavigationControl_Base::XValue()
{
    float xValue = fmin(fmax((_X - _baseX) * _scale, -1.0f), 1.0f);
    return xValue;
}

float TurboSceneNavigationControl_Base::YValue()
{
    float yValue = fmin(fmax((_Y - _baseY) * _scale, -1.0f), 1.0f);
    return yValue;
}

bool TurboSceneNavigationControl_Base::Contains(float x, float y, float z)
{
    return (_minX <= x && x < _maxX && _minY <= y && y < _maxY);
}

void TurboSceneNavigationControl_Base::CurrentPoint(float x, float y, float z)
{
    if (_firstPoint)
    {
        _baseX = x;
        _baseY = y;
        _firstPoint = false;
    }

    _X = x;
    _Y = y;
}
