
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>
#include <ITurboSceneSprite.h>

using namespace Turbo::Scene;

namespace Turbo
{
    namespace View
    {
        class ITurboControlViewModel
        {
        public:
            //	ITurboControlViewModel Properties ----------------------------------------------------------------------
            virtual bool IsActive() = 0;
            virtual void IsActive(bool isActive) = 0;
            virtual bool IsActivated() = 0;

            virtual std::shared_ptr<ITurboSceneNavigationControl> Control() = 0;

            //	ITurboControlViewModel Methods -------------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z = 0) = 0;
        };
    }
}