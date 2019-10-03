
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>
#include <ITurboSceneNavigationTouch.h>
#include <ITurboDebug.h>
#include "ITurboSceneTexture.h"

using namespace Turbo::Core::Debug;

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationControl_Last :
                public ITurboSceneNavigationControl,
                public ITurboSceneNavigationTouch
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSceneNavigationControl_Last(
                    std::shared_ptr<ITurboDebug> debug,
                    TurboGameControlType type,
                    float minX, float maxX,
                    float minY, float maxY,
                    float scale = 1.0f,
                    std::string textureName = "");
            virtual ~TurboSceneNavigationControl_Last(){}

            //  ITurboSceneNavigationControl Properties  --------------------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive);
            virtual float XValue();
            virtual float YValue();
            virtual float ZValue() { return 0.0f; }

            //  ITurboSceneNavigationControl Methods  -----------------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z = 0);

            //  ITurboSceneNavigationTouch Properties  ----------------------------------------------------------------------------
            virtual float MinX() { return _minX; }
            virtual float MaxX() { return _maxX; }
            virtual float MinY() { return _minY; }
            virtual float MaxY() { return _maxY; }

            virtual std::shared_ptr<ITurboSceneTexture> Texture() { return _texture; }
            virtual void Texture(std::shared_ptr<ITurboSceneTexture> texture) { _texture = texture; }

            //  ITurboSceneNavigationTouch Methods  -------------------------------------------------------------------------------
            virtual bool Contains(float x, float y);

        private:
            std::shared_ptr<ITurboDebug> _debug;
            TurboGameControlType _type;
            float _minX;
            float _maxX;
            float _minY;
            float _maxY;
            float _scale = 1.0f;
            std::shared_ptr<ITurboSceneTexture> _texture = nullptr;

            bool _isActive = false;
            bool _firstPoint = false;
            float _lastX = 0.0f;
            float _lastY = 0.0f;
            float _X = 0.0f;
            float _Y = 0.0f;
        };
    }
}