
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>
#include "ITurboSceneNavigationControl.h"

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationControl_Button : public ITurboSceneNavigationControl
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSceneNavigationControl_Button(
                    TurboGameControlType type,
                    float minX,
                    float maxX,
                    float minY,
                    float maxY);
            virtual ~TurboSceneNavigationControl_Button(){}

            //  ITurboSceneNavigationControl Properties  --------------------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive) { _isActive = isActive; }
            virtual float XValue() { return 0.0f; }
            virtual float YValue() { return -1.0f; }
            virtual float ZValue() { return 0.0f; }

            //  ITurboSceneNavigationControl Methods  -----------------------------------------------------------------------------
            virtual bool Contains(float x, float y, float z = 0);
            virtual void CurrentPoint(float x, float y, float z = 0) {}

        private:
            TurboGameControlType _type;
            float _minX;
            float _maxX;
            float _minY;
            float _maxY;

            bool _isActive = false;
        };
    }
}