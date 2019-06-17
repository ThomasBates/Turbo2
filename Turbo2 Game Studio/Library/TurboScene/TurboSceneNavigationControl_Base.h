
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>
#include "ITurboSceneNavigationControl.h"

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationControl_Base : public ITurboSceneNavigationControl
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSceneNavigationControl_Base(
                    TurboGameControlType type,
                    float minX, float maxX,
                    float minY, float maxY,
                    float scale = 1.0);
            virtual ~TurboSceneNavigationControl_Base(){}

            //  ITurboSceneNavigationControl Properties  --------------------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive);
            virtual float XValue();
            virtual float YValue();
            virtual float ZValue() { return 0.0f; }

            //  ITurboSceneNavigationControl Methods  -----------------------------------------------------------------------------
            virtual bool Contains(float x, float y, float z = 0);
            virtual void CurrentPoint(float x, float y, float z = 0);

        private:
            TurboGameControlType _type;
            float _minX;
            float _maxX;
            float _minY;
            float _maxY;
            float _scale;

            bool _isActive = false;
            bool _firstPoint = false;
            float _baseX = 0.0f;
            float _baseY = 0.0f;
            float _X = 0.0f;
            float _Y = 0.0f;
        };
    }
}