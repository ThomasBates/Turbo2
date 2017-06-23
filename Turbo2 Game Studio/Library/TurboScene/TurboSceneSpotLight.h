#pragma once

#include <pch.h>

#include <ITurboSceneLight.h>

using namespace Turbo::Graphics;
using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneSpotLight : public ITurboSceneLight
		{
		public:
			//	Constructors -----------------------------------------------------------------------------------------------------
			TurboSceneSpotLight(
				TurboVector3D position,
				TurboVector3D direction,
				TurboColor color,
				float constantAttenuation,
				float linearAttenuation,
				float quadraticAttenuation,
				float hotSpotAngle,
				float fallOffAngle) :
				_position(position),
				_direction(direction),
				_color(color),
				_constantAttenuation(constantAttenuation),
				_linearAttenuation(linearAttenuation),
				_quadraticAttenuation(quadraticAttenuation),
				_hotSpotAngle(hotSpotAngle),
				_fallOffAngle(fallOffAngle)
			{
			}

			//	ITurboSceneLight Properties --------------------------------------------------------------------------------------
			virtual TurboSceneLightType LightType() { return Point; }

			virtual TurboVector3D Position() { return _position; }
			virtual void Position(TurboVector3D position) { _position = position; }

			virtual TurboVector3D Direction() { return _direction; }
			virtual void Direction(TurboVector3D direction) { _direction = direction; }

			virtual TurboColor Color() { return _color; }
			virtual void Color(TurboColor color) { _color = color; }

			virtual float ConstantAttenuation() { return _constantAttenuation; }
			virtual void ConstantAttenuation(float constantAttenuation) { _constantAttenuation = constantAttenuation; }

			virtual float LinearAttenuation() { return _linearAttenuation; }
			virtual void LinearAttenuation(float linearAttenuation) { _linearAttenuation = linearAttenuation; }

			virtual float QuadraticAttenuation() { return _quadraticAttenuation; }
			virtual void QuadraticAttenuation(float quadraticAttenuation) { _quadraticAttenuation = quadraticAttenuation; }

			virtual float HotSpotAngle() { return _hotSpotAngle; }
			virtual void HotSpotAngle(float hotSpotAngle) { _hotSpotAngle = hotSpotAngle; }

			virtual float FallOffAngle() { return _fallOffAngle; }
			virtual void FallOffAngle(float fallOffAngle) { _fallOffAngle = fallOffAngle; }

		private:
			//	Private Members --------------------------------------------------------------------------------------------------
			TurboVector3D _position;
			TurboVector3D _direction;
			TurboColor _color;
			float _constantAttenuation;
			float _linearAttenuation;
			float _quadraticAttenuation;
			float _hotSpotAngle;
			float _fallOffAngle;
		};
	}
}