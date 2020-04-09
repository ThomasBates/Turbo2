#pragma once

#include <pch.h>

#include <ITurboSceneTexture.h>
#include <TurboColor.h>
#include <TurboVector2D.h>
#include <TurboRectangle.h>

using namespace Turbo::Graphics;
using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneSprite
		{
		public:
			//  ITurboSceneSprite Properties ---------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboSceneTexture> Texture() = 0;
			virtual void Texture(std::shared_ptr<ITurboSceneTexture> texture) = 0;

			virtual TurboColor Color() = 0;
			virtual void Color(TurboColor color) = 0;

			virtual bool UseRectangle() = 0;
			virtual void UseRectangle(bool useRectangle) = 0;

			virtual TurboRectangle Rectangle() = 0;
			virtual void Rectangle(TurboRectangle rectangle) = 0;

			virtual TurboVector2D Location() = 0;
			virtual void Location(TurboVector2D location) = 0;

			virtual TurboVector2D Origin() = 0;
			virtual void Origin(TurboVector2D origin) = 0;

			virtual float Rotation() = 0;
			virtual void Rotation(float rotation) = 0;

			virtual TurboVector2D Scale() = 0;
			virtual void Scale(TurboVector2D scale) = 0;
		};
	}
}
