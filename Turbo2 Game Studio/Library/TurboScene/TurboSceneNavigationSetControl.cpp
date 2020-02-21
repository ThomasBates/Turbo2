
#include <pch.h>

#include <TurboSceneNavigationSetControl.h>

using namespace Turbo::Scene;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

TurboSceneNavigationSetControl::TurboSceneNavigationSetControl(TurboGameControlType type) :
        _type(type)
{
}

//  ITurboSceneNavigationControl Methods  ------------------------------------------------------------------------------

void TurboSceneNavigationSetControl::CurrentPoint(float x, float y, float z)
{
    _xValue = x;
    _yValue = y;
	_zValue = z;
}
