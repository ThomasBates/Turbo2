
#include <pch.h>

#include <TurboSceneNavigationTapControl.h>

using namespace Turbo::Scene;

TurboSceneNavigationTapControl::TurboSceneNavigationTapControl(
        TurboGameControlType type,
        float xValue,
        float yValue,
        float zValue) :
        _type(type),
        _xValue(xValue),
        _yValue(yValue),
        _zValue(zValue)
{
}
