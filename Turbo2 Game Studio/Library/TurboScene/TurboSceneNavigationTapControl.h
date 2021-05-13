
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationTapControl : public ITurboSceneNavigationControl
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSceneNavigationTapControl(
                    TurboGameControlType type,
                    float xValue,
                    float yValue,
                    float zValue);
            virtual ~TurboSceneNavigationTapControl(){}

            //  ITurboSceneNavigationControl Properties  --------------------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive) { _isActive = isActive; }
            virtual float XValue() { return _xValue; }
            virtual float YValue() { return _yValue; }
            virtual float ZValue() { return _zValue; }

            //  ITurboSceneNavigationControl Methods  -----------------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z = 0) {}

        private:
            TurboGameControlType _type;
            float _xValue;
            float _yValue;
            float _zValue;

            bool _isActive = false;
        };
    }
}