
#include <pch.h>

#include <TurboSceneNavigationControl_Button.h>

using namespace Turbo::Scene;

TurboSceneNavigationControl_Button::TurboSceneNavigationControl_Button(
        TurboGameControlType type,
        float minX,
        float maxX,
        float minY,
        float maxY) :
        _type(type),
        _minX(minX),
        _maxX(maxX),
        _minY(minY),
        _maxY(maxY)
{
}

bool TurboSceneNavigationControl_Button::Contains(float x, float y, float z)
{
    return (_minX <= x && x < _maxX && _minY <= y && y < _maxY);
}
