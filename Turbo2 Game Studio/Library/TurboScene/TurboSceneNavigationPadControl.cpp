
#include <pch.h>

#include <TurboSceneNavigationPadControl.h>

using namespace Turbo::Scene;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

TurboSceneNavigationPadControl::TurboSceneNavigationPadControl(
        TurboGameControlType type,
        float scale) :
        _type(type),
        _scale(scale)
{
}

//  ITurboSceneNavigationControl Properties ----------------------------------------------------------------------------

void TurboSceneNavigationPadControl::IsActive(bool isActive)
{
    if (_isActive != isActive)
    {
        _isActive = isActive;
        _firstPoint = true;
    }
}

float TurboSceneNavigationPadControl::XValue()
{
    float xValue = (_X - _lastX) * _scale;

    _lastX = _X;
    return xValue;
}

float TurboSceneNavigationPadControl::YValue()
{
    float yValue = (_Y - _lastY) * _scale;

    _lastY = _Y;
    return yValue;
}

//  ITurboSceneNavigationControl Methods -------------------------------------------------------------------------------

void TurboSceneNavigationPadControl::CurrentPoint(float x, float y, float z)
{
    if (_firstPoint)
    {
        _lastX = x;
        _lastY = y;
        _firstPoint = false;
    }

    _X = x;
    _Y = y;
}
