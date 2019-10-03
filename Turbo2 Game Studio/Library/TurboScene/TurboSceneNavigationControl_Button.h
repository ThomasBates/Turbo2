
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
                    float maxY,
                    float xValue,
                    float yValue,
                    float zValue,
                    std::string textureName = "");
            virtual ~TurboSceneNavigationControl_Button(){}

            //  ITurboSceneNavigationControl Properties  --------------------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive) { _isActive = isActive; }
            virtual float XValue() { return _xValue; }
            virtual float YValue() { return _yValue; }
            virtual float ZValue() { return _zValue; }

            //  ITurboSceneNavigationControl Methods  -----------------------------------------------------------------------------
            virtual void CurrentPoint(float x, float y, float z = 0) {}

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
            TurboGameControlType _type;
            float _minX;
            float _maxX;
            float _minY;
            float _maxY;
            float _xValue;
            float _yValue;
            float _zValue;
            std::shared_ptr<ITurboSceneTexture> _texture = nullptr;

            bool _isActive = false;
        };
    }
}