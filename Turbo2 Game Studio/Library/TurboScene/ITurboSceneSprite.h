#pragma once

#include <ITurboSceneTexture.h>
#include <ITurboScenePlacement.h>

#include <TurboColor.h>
#include <TurboSceneNavigationInfo.h>

using namespace Turbo::Graphics;

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneSprite
		{
		public:
			//  ITurboSceneSprite Properties -----------------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboSceneTexture> Texture() = 0;
			virtual void Texture(std::shared_ptr<ITurboSceneTexture> texture) = 0;

			virtual TurboColor Color() = 0;
			virtual void Color(TurboColor color) = 0;

			virtual bool UseRectangle() = 0;
			virtual void UseRectangle(bool useRectangle) = 0;

			virtual float Left() = 0;
			virtual void Left(float left) = 0;

			virtual float Top() = 0;
			virtual void Top(float top) = 0;

			virtual float Right() = 0;
			virtual void Right(float right) = 0;

			virtual float Bottom() = 0;
			virtual void Bottom(float bottom) = 0;

			virtual TurboVector2D Location() = 0;
			virtual void Location(TurboVector2D location) = 0;

			virtual TurboVector2D Origin() = 0;
			virtual void Origin(TurboVector2D origin) = 0;

			virtual float Rotation() = 0;
			virtual void Rotation(float rotation) = 0;

			virtual float Scale() = 0;
			virtual void Scale(float scale) = 0;

			//  ITurboSceneSprite Methods --------------------------------------------------------------------------------------
			virtual void Update(NavigationInfo navInfo) = 0;
		};
	}
}
