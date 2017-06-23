#pragma once

#include <pch.h>

#include <ITurboSceneLight.h>

using namespace Turbo::Graphics;
using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		class TurboScenePointLight : public ITurboSceneLight
		{
		public:
			//	Constructors -----------------------------------------------------------------------------------------------------
			TurboScenePointLight(
				TurboVector3D position, 
				TurboColor color,
				float constantAttenuation,
				float linearAttenuation,
				float quadraticAttenuation) :
				_position(position),
				_color(color),
				_constantAttenuation(constantAttenuation),
				_linearAttenuation(linearAttenuation),
				_quadraticAttenuation(quadraticAttenuation)
			{
			}

			//	ITurboSceneLight Properties --------------------------------------------------------------------------------------
			virtual TurboSceneLightType LightType() { return Point; }

			virtual TurboVector3D Position() { return _position; }
			virtual void Position(TurboVector3D position) { _position = position; }

			virtual TurboVector3D Direction() { return _direction; }
			virtual void Direction(TurboVector3D direction) {}

			virtual TurboColor Color() { return _color; }
			virtual void Color(TurboColor color) { _color = color; }

			virtual float ConstantAttenuation() { return _constantAttenuation; }
			virtual void ConstantAttenuation(float constantAttenuation) { _constantAttenuation = constantAttenuation; }

			virtual float LinearAttenuation() { return _linearAttenuation; }
			virtual void LinearAttenuation(float linearAttenuation) { _linearAttenuation = linearAttenuation; }

			virtual float QuadraticAttenuation() { return _quadraticAttenuation; }
			virtual void QuadraticAttenuation(float quadraticAttenuation) { _quadraticAttenuation = quadraticAttenuation; }

			virtual float HotSpotAngle() { return 0; }
			virtual void HotSpotAngle(float hotSpotAngle) {}

			virtual float FallOffAngle() { return 0; }
			virtual void FallOffAngle(float fallOffAngle) {}

		private:
			//	Private Members --------------------------------------------------------------------------------------------------
			TurboVector3D _position;
			TurboVector3D _direction;
			TurboColor _color;
			float _constantAttenuation;
			float _linearAttenuation;
			float _quadraticAttenuation;
		};
	}
}