
#include <pch.h>

#include <TurboSceneNavigationStickControl.h>

using namespace Turbo::Scene;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

TurboSceneNavigationStickControl::TurboSceneNavigationStickControl(
        TurboGameControlType type,
        float scale) :
        _type(type),
        _scale(scale)
{
}

//  ITurboSceneNavigationControl Properties ----------------------------------------------------------------------------

void TurboSceneNavigationStickControl::IsActive(bool isActive)
{
    if (_isActive != isActive)
    {
        _isActive = isActive;
        _firstPoint = isActive;
    }
}

float TurboSceneNavigationStickControl::XValue()
{
    float xValue = fmin(fmax((_X - _baseX) * _scale, -1.0F), 1.0F);
    return xValue;
}

float TurboSceneNavigationStickControl::YValue()
{
    float yValue = fmin(fmax((_Y - _baseY) * _scale, -1.0F), 1.0F);
    return yValue;
}

//  ITurboSceneNavigationControl Methods -------------------------------------------------------------------------------

void TurboSceneNavigationStickControl::CurrentPoint(float x, float y, float z)
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
