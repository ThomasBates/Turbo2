
#pragma once

#include <pch.h>

#include <ITurboSceneTexture.h>
#include <TurboSceneTypes.h>

namespace Turbo
{
    namespace Scene
    {
        class ITurboSceneNavigationTouch
        {
        public:
            virtual ~ITurboSceneNavigationTouch(){}

            //  ITurboSceneNavigationTouch Properties  -----------------------------------------------------------------
            virtual float MinX() = 0;
            virtual float MaxX() = 0;
            virtual float MinY() = 0;
            virtual float MaxY() = 0;

            virtual std::shared_ptr<ITurboSceneTexture> Texture() = 0;
            virtual void Texture(std::shared_ptr<ITurboSceneTexture>) = 0;

            //  ITurboSceneNavigationTouch Methods  --------------------------------------------------------------------
            virtual bool Contains(float x, float y) = 0;
        };
    }
}