
#include <pch.h>

#include <TurboSceneNavigationControl_Button.h>
#include <TurboSceneTexture.h>

using namespace Turbo::Scene;

TurboSceneNavigationControl_Button::TurboSceneNavigationControl_Button(
        TurboGameControlType type,
        float minX,
        float maxX,
        float minY,
        float maxY,
        float xValue,
        float yValue,
        float zValue,
        std::string textureName) :
        _type(type),
        _minX(minX),
        _maxX(maxX),
        _minY(minY),
        _maxY(maxY),
        _xValue(xValue),
        _yValue(yValue),
        _zValue(zValue)
{
    if (!textureName.empty())
    {
        _texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture(textureName));
    }
}

bool TurboSceneNavigationControl_Button::Contains(float x, float y)
{
    return (_minX <= x && x < _maxX &&
            _minY <= y && y < _maxY);
}
