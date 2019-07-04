
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>
#include <ITurboSceneNavigationTouch.h>

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationControl_Button :
                public ITurboSceneNavigationControl,
                public ITurboSceneNavigationTouch
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
            virtual void CurrentPoint(float x, float y, float z = 0) {}

            //  ITurboSceneNavigationTouch Properties  ----------------------------------------------------------------------------
            virtual float MinX() { return _minX; }
            virtual float MaxX() { return _maxX; }
            virtual float MinY() { return _minY; }
            virtual float MaxY() { return _maxY; }

            //  ITurboSceneNavigationTouch Methods  -------------------------------------------------------------------------------
            virtual bool Contains(float x, float y);

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