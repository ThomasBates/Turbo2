
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationSetControl : public ITurboSceneNavigationControl
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSceneNavigationSetControl(TurboGameControlType type);
            virtual ~TurboSceneNavigationSetControl() {}

            //  ITurboSceneNavigationControl Properties  ---------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive) { _isActive = isActive; }
            virtual float XValue() { return _xValue; }
            virtual float YValue() { return _yValue; }
			virtual float ZValue() { return _zValue; }

            //  ITurboSceneNavigationControl Methods  ------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z);

        private:
            TurboGameControlType _type;

            bool _isActive = false;
            float _xValue = 0.0f;
            float _yValue = 0.0f;
			float _zValue = 0.0f;
        };
    }
}