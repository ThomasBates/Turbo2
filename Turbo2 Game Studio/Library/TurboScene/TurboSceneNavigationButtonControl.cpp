
#include <pch.h>

#include <TurboSceneNavigationButtonControl.h>

using namespace Turbo::Scene;

TurboSceneNavigationButtonControl::TurboSceneNavigationButtonControl(
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
