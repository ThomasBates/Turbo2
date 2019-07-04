
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationControl_Set :
                public ITurboSceneNavigationControl
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSceneNavigationControl_Set(TurboGameControlType type);
            virtual ~TurboSceneNavigationControl_Set() {}

            //  ITurboSceneNavigationControl Properties  --------------------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive);
            virtual float XValue();
            virtual float YValue();
			virtual float ZValue();

            //  ITurboSceneNavigationControl Methods  -----------------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z);

        private:
            TurboGameControlType _type;

            bool _isActive = false;
            float _X = 0.0f;
            float _Y = 0.0f;
			float _Z = 0.0f;
        };
    }
}