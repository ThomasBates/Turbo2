
#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>
#include <ITurboDebug.h>

using namespace Turbo::Core::Debug;

namespace Turbo
{
    namespace Scene
    {
        class TurboSceneNavigationControl_Set : public ITurboSceneNavigationControl
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboSceneNavigationControl_Set(
                    TurboGameControlType type,
                    float minX, float maxX,
                    float minY, float maxY);
            virtual ~TurboSceneNavigationControl_Set(){}

            //  ITurboSceneNavigationControl Properties  --------------------------------------------------------------------------
            virtual TurboGameControlType Type() { return _type; }
            virtual bool IsActive() { return _isActive; }
            virtual void IsActive(bool isActive);
            virtual float XValue();
            virtual float YValue();
			virtual float ZValue();

            //  ITurboSceneNavigationControl Methods  -----------------------------------------------------------------------------
            virtual bool Contains(float x, float y, float z = 0);
            virtual void CurrentPoint(float x, float y, float z = 0);

        private:
            TurboGameControlType _type;
            float _minX;
            float _maxX;
            float _minY;
            float _maxY;

            bool _isActive = false;
            float _X = 0.0f;
            float _Y = 0.0f;
			float _Z = 0.0f;
        };
    }
}