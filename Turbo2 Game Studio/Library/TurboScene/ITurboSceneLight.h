#pragma once

#include<pch.h>

#include <TurboColor.h>
#include <TurboVector3D.h>

using namespace Turbo::Graphics;
using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		//	https://courses.cs.washington.edu/courses/cse458/05au/reading/lighting_tutorial/light_types.html
		enum TurboSceneLightType
		{
			Ambient,		//	No Position, No Direction
			Point,			//	Position, No Direction
			Directional,	//	No Position, Direction
			Spot,			//	Position, Direction
			Area,
			Volume,
		};

		class ITurboSceneLight
		{
		public:
			virtual TurboSceneLightType LightType() = 0;

			virtual TurboVector3D Position() = 0;
			virtual void Position(TurboVector3D position) = 0;

			virtual TurboVector3D Direction() = 0;
			virtual void Direction(TurboVector3D direction) = 0;

			virtual TurboColor Color() = 0;
			virtual void Color(TurboColor color) = 0;

			virtual float ConstantAttenuation() = 0;
			virtual void ConstantAttenuation(float constantAttenuation) = 0;

			virtual float LinearAttenuation() = 0;
			virtual void LinearAttenuation(float linearAttenuation) = 0;

			virtual float QuadraticAttenuation() = 0;
			virtual void QuadraticAttenuation(float quadraticAttenuation) = 0;

			virtual float HotSpotAngle() = 0;
			virtual void HotSpotAngle(float hotSpotAngle) = 0;

			virtual float FallOffAngle() = 0;
			virtual void FallOffAngle(float fallOffAngle) = 0;
		};
	}
}