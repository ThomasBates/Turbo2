#pragma once

#include <ITurboSceneText.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneText : public ITurboSceneText
		{
		public:
			//  Constructors and Destructors -----------------------------------------------------------------------------------
			TurboSceneText();
			TurboSceneText(std::shared_ptr<ITurboSceneFont> font, std::string text);

			//  ITurboSceneText Properties -----------------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboSceneFont> Font() { return _font; }
			virtual void Font(std::shared_ptr<ITurboSceneFont> font) { _font = font; }

			virtual std::string Text() { return _text; }
			virtual void Text(std::string text) { _text = text; }

			virtual TurboColor Color() { return _color; }
			virtual void Color(TurboColor color) { _color = color; }

			virtual float BorderSize() { return _borderSize; }
			virtual void BorderSize(float borderSize) { _borderSize = borderSize; }

			virtual TurboColor BorderColor() { return _borderColor; }
			virtual void BorderColor(TurboColor borderColor) { _borderColor = borderColor; }

			virtual TurboVector2D Location() { return _location; }
			virtual void Location(TurboVector2D location) { _location = location; }

			virtual TurboVector2D Origin() { return _origin; }
			virtual void Origin(TurboVector2D origin) { _origin = origin; }

			virtual float Rotation() { return _rotation; }
			virtual void Rotation(float rotation) { _rotation = rotation; }

			virtual float Scale() { return _scale; }
			virtual void Scale(float scale) { _scale = scale; }

			//  ITurboSceneText Methods --------------------------------------------------------------------------------------
			virtual void Update(NavigationInfo navInfo) {}

		private:
			std::shared_ptr<ITurboSceneFont>	_font;
			std::string							_text;
			TurboColor							_color = TurboColor(0.0f, 0.0f, 0.0f, 1.0f);
			float								_borderSize = 0.f;
			TurboColor							_borderColor = TurboColor(0.0f, 0.0f, 0.0f, 1.0f);
			TurboVector2D						_location = TurboVector2D(0.0f, 0.0f);
			TurboVector2D						_origin = TurboVector2D(0.0f, 0.0f);
			float								_rotation = 0.0f;
			float								_scale = 1.0f;
		};
	}
}
