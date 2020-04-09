#pragma once

#include <pch.h>

#include <TurboColor.h>
#include <TurboVector2D.h>
#include <TurboRectangle.h>
#include <TurboSceneTypes.h>

using namespace Turbo::Graphics;
using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneText
		{
		public:
			//  ITurboSceneText Properties -----------------------------------------------------------------------------
			virtual std::string FontName() = 0;
			virtual void FontName(std::string fontName) = 0;

			virtual float FontSize() = 0;
			virtual void FontSize(float fontSize) = 0;

			virtual TurboColor Color() = 0;
			virtual void Color(TurboColor color) = 0;

			virtual SceneTextHorizontalAlignment HorizontalAlignment() = 0;
			virtual void HorizontalAlignment(SceneTextHorizontalAlignment horizontalAlignment) = 0;

			virtual SceneTextVerticalAlignment VerticalAlignment() = 0;
			virtual void VerticalAlignment(SceneTextVerticalAlignment verticalAlignment) = 0;

			virtual std::string Text() = 0;
			virtual void Text(std::string text) = 0;

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

			virtual bool PropertyChanged() = 0;
			virtual void PropertyChanged(bool useRectangle) = 0;
		};
	}
}
