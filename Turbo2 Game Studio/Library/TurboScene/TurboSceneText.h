#pragma once

#include <ITurboSceneText.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneText : public ITurboSceneText
		{
		public:
			//  Constructors and Destructors ---------------------------------------------------------------------------
			TurboSceneText(std::string fontName, float fontSize, TurboColor color, std::string text = "");
			virtual ~TurboSceneText() {}

			//  ITurboSceneText Properties -----------------------------------------------------------------------------
			virtual std::string FontName() { return _fontName; }
			virtual void FontName(std::string fontName);

			virtual float FontSize() { return _fontSize; }
			virtual void FontSize(float fontSize);

			virtual TurboColor Color() { return _color; }
			virtual void Color(TurboColor color);

			virtual SceneTextHorizontalAlignment HorizontalAlignment() { return _horizontalAlignment; }
			virtual void HorizontalAlignment(SceneTextHorizontalAlignment horizontalAlignment);

			virtual SceneTextVerticalAlignment VerticalAlignment() { return _verticalAlignment; }
			virtual void VerticalAlignment(SceneTextVerticalAlignment verticalAlignment);

			virtual std::string Text() { return _text; }
			virtual void Text(std::string text);

			virtual bool UseRectangle() { return _useRectangle; }
			virtual void UseRectangle(bool useRectangle);

			virtual TurboRectangle Rectangle() { return _rectangle; }
			virtual void Rectangle(TurboRectangle rectangle);

			virtual TurboVector2D Location() { return _location; }
			virtual void Location(TurboVector2D location);

			virtual TurboVector2D Origin() { return _origin; }
			virtual void Origin(TurboVector2D origin);

			virtual float Rotation() { return _rotation; }
			virtual void Rotation(float rotation);

			virtual TurboVector2D Scale() { return _scale; }
			virtual void Scale(TurboVector2D scale);

			virtual bool PropertyChanged() { return _propertyChanged; }
			virtual void PropertyChanged(bool propertyChanged)
			{
                _propertyChanged = propertyChanged;
            }

		private:
			std::string _fontName;
			float _fontSize;
			TurboColor _color {1, 1, 1, 1};
			SceneTextHorizontalAlignment _horizontalAlignment;
			SceneTextVerticalAlignment _verticalAlignment;

			std::string _text;

			bool _useRectangle = true;
			TurboRectangle _rectangle {};

			TurboVector2D _location {};
			TurboVector2D _origin {};
			float _rotation = 0;
			TurboVector2D _scale {1, 1};

			bool _propertyChanged = true;

			void NotifyPropertyChanged() { _propertyChanged = true; }
		};
	}
}
