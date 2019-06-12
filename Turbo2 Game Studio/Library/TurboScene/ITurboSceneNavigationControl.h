
#pragma once

#include <TurboSceneTypes.h>

namespace Turbo
{
    namespace Scene
    {
        class ITurboSceneNavigationControl
        {
        public:
            //  ITurboSceneNavigationControl Properties  ---------------------------------------------------------------
            virtual TurboGameControlType Type() = 0;
            virtual bool IsActive() = 0;
            virtual void IsActive(bool isActive) = 0;
            virtual float XValue() = 0;
            virtual float YValue() = 0;
            virtual float ZValue() = 0;

            //  ITurboSceneNavigationControl Methods  ------------------------------------------------------------------
            virtual bool Contains(float x, float y, float z = 0) = 0;
            virtual void CurrentPoint(float x, float y, float z = 0) = 0;
        };
    }
}