
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationStickControl : public ITurboSceneNavigationControl
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSceneNavigationStickControl(
                    TurboGameControlType type,
                    float scale = 1.0);
            virtual ~TurboSceneNavigationStickControl(){}

            //  ITurboSceneNavigationControl Properties ----------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive);
            virtual float XValue();
            virtual float YValue();
            virtual float ZValue() { return 0.0f; }

            //  ITurboSceneNavigationControl Methods -------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z);

        private:
            TurboGameControlType _type;
            float _scale = 1.0f;

            bool _isActive = false;
            bool _firstPoint = false;
            float _baseX = 0.0f;
            float _baseY = 0.0f;
            float _X = 0.0f;
            float _Y = 0.0f;
        };
    }
}