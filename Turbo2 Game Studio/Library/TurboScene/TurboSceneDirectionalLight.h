#pragma once

#include <pch.h>

#include <ITurboSceneLight.h>

using namespace Turbo::Graphics;
using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneDirectionalLight : public ITurboSceneLight
		{
		public:
			//	Constructors -----------------------------------------------------------------------------------------------------
			TurboSceneDirectionalLight(TurboVector3D direction, TurboColor color) :
				_direction(direction),
				_color(color)
			{
			}

			//	ITurboSceneLight Properties --------------------------------------------------------------------------------------
			virtual TurboSceneLightType LightType() { return Directional; }

			virtual TurboVector3D Position() { return _position; }
			virtual void Position(TurboVector3D position) {}

			virtual TurboVector3D Direction() { return _direction; }
			virtual void Direction(TurboVector3D direction) { _direction = direction; }

			virtual TurboColor Color() { return _color; }
			virtual void Color(TurboColor color) { _color = color; }

			virtual float ConstantAttenuation() { return 0; }
			virtual void ConstantAttenuation(float constantAttenuation) {}

			virtual float LinearAttenuation() { return 0; }
			virtual void LinearAttenuation(float linearAttenuation) {}

			virtual float QuadraticAttenuation() { return 0; }
			virtual void QuadraticAttenuation(float quadraticAttenuation) {}

			virtual float HotSpotAngle() { return 0; }
			virtual void HotSpotAngle(float hotSpotAngle) {}

			virtual float FallOffAngle() { return 0; }
			virtual void FallOffAngle(float fallOffAngle) {}

		private:
			//	Private Members --------------------------------------------------------------------------------------------------
			TurboVector3D _position;
			TurboVector3D _direction;
			TurboColor _color;
		};
	}
}