
#include <pch.h>

#include "TurboSceneNavigationControl_Last.h"

using namespace Turbo::Core::Debug;
using namespace Turbo::Scene;

TurboSceneNavigationControl_Last::TurboSceneNavigationControl_Last(
        std::shared_ptr<ITurboDebug> debug,
        TurboGameControlType type,
        float minX, float maxX,
        float minY, float maxY,
        float scale) :
        _debug(debug),
        _type(type),
        _minX(minX), _maxX(maxX),
        _minY(minY), _maxY(maxY),
        _scale(scale)
{
}

void TurboSceneNavigationControl_Last::IsActive(bool isActive)
{
    if (_isActive != isActive)
    {
        _isActive = isActive;
        _firstPoint = true;
    }
}

float TurboSceneNavigationControl_Last::XValue()
{
    float xValue = (_X - _lastX) * _scale;

    //_debug->Send(TurboDebugSeverity::debugDebug, TurboDebugCategory::debugController) << "TurboSceneNavigationControl_Last::XValue(): " << _lastX << " -->> " << _X << ": " << xValue << "\n";

    _lastX = _X;
    return xValue;
}

float TurboSceneNavigationControl_Last::YValue()
{
    float yValue = (_Y - _lastY) * _scale;

    //_debug->Send(TurboDebugSeverity::debugDebug, TurboDebugCategory::debugController) << "TurboSceneNavigationControl_Last::YValue(): " << _lastY << " -->> " << _Y << ": " << yValue << "\n";

    _lastY = _Y;
    return yValue;
}

bool TurboSceneNavigationControl_Last::Contains(float x, float y, float z)
{
    return (_minX <= x && x < _maxX && _minY <= y && y < _maxY);
}

void TurboSceneNavigationControl_Last::CurrentPoint(float x, float y, float z)
{
    //_debug->Send(TurboDebugSeverity::debugDebug, TurboDebugCategory::debugController) << "TurboSceneNavigationControl_Last::CurrentPoint(" << x << ", " << y << ")\n";

    if (_firstPoint)
    {
        //_debug->Send(TurboDebugSeverity::debugDebug, TurboDebugCategory::debugController) << "TurboSceneNavigationControl_Last::CurrentPoint(): _firstPoint\n";

        _lastX = x;
        _lastY = y;
        _firstPoint = false;
    }

    _X = x;
    _Y = y;
}
