#pragma once

#include <ITurboSceneFont.h>
#include <ITurboScenePlacement.h>

#include <TurboColor.h>
#include <TurboSceneTypes.h>

using namespace Turbo::Graphics;

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneText
		{
		public:
			//  ITurboSceneText Properties -----------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboSceneFont> Font() = 0;
			virtual void Font(std::shared_ptr<ITurboSceneFont> Font) = 0;

			virtual std::string Text() = 0;
			virtual void Text(std::string text) = 0;

			virtual TurboColor Color() = 0;
			virtual void Color(TurboColor color) = 0;

			virtual float BorderSize() = 0;
			virtual void BorderSize(float borderSize) = 0;

			virtual TurboColor BorderColor() = 0;
			virtual void BorderColor(TurboColor borderColor) = 0;

			virtual TurboVector2D Location() = 0;
			virtual void Location(TurboVector2D location) = 0;

			virtual TurboVector2D Origin() = 0;
			virtual void Origin(TurboVector2D origin) = 0;

			virtual float Rotation() = 0;
			virtual void Rotation(float rotation) = 0;

			virtual float Scale() = 0;
			virtual void Scale(float scale) = 0;

			//  ITurboSceneText Methods --------------------------------------------------------------------------------
		};
	}
}
