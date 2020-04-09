
#include <pch.h>

#include <TurboSceneText.h>

using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

TurboSceneText::TurboSceneText(std::string fontName, float fontSize, TurboColor color, std::string text) :
	_fontName(fontName),
	_fontSize(fontSize),
	_color(color),
	_text(text)
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneText Properties -----------------------------------------------------------------------------------------

void TurboSceneText::FontName(std::string fontName)
{
	if (_fontName == fontName)
		return;

	_fontName = fontName;
	NotifyPropertyChanged();
}

void TurboSceneText::FontSize(float fontSize)
{
	if (_fontSize == fontSize)
		return;

	_fontSize = fontSize;
	NotifyPropertyChanged();
}

void TurboSceneText::Color(TurboColor color)
{
	if (_color.R == color.R &&
	    _color.G == color.G &&
	    _color.B == color.B &&
	    _color.A == color.A)
	{
		return;
	}

	_color = color;
	NotifyPropertyChanged();
}

void TurboSceneText::HorizontalAlignment(SceneTextHorizontalAlignment horizontalAlignment)
{
	if (_horizontalAlignment == horizontalAlignment)
		return;

	_horizontalAlignment = horizontalAlignment;
	NotifyPropertyChanged();
}

void TurboSceneText::VerticalAlignment(SceneTextVerticalAlignment verticalAlignment)
{
	if (_verticalAlignment == verticalAlignment)
		return;

	_verticalAlignment = verticalAlignment;
	NotifyPropertyChanged();
}

void TurboSceneText::Text(std::string text)
{
	if (_text == text)
		return;

	_text = text;
	NotifyPropertyChanged();
}

void TurboSceneText::UseRectangle(bool useRectangle)
{
	if (_useRectangle == useRectangle)
		return;

	_useRectangle = useRectangle;
	NotifyPropertyChanged();
}

void TurboSceneText::Rectangle(TurboRectangle rectangle)
{
	if (_rectangle.X1 == rectangle.X1 &&
		_rectangle.Y1 == rectangle.Y1 &&
		_rectangle.X2 == rectangle.X2 &&
		_rectangle.Y2 == rectangle.Y2)
	{
		return;
	}

	_rectangle = rectangle;
	NotifyPropertyChanged();
}

void TurboSceneText::Location(TurboVector2D location)
{
	if (_location.X == location.X &&
		_location.Y == location.Y)
	{
		return;
	}

	_location = location;
	NotifyPropertyChanged();
}

void TurboSceneText::Origin(TurboVector2D origin)
{
	if (_origin.X == origin.X &&
		_origin.Y == origin.Y)
	{
		return;
	}

	_origin = origin;
	NotifyPropertyChanged();
}

void TurboSceneText::Rotation(float rotation)
{
	if (_rotation == rotation)
		return;

	_rotation = rotation;
	NotifyPropertyChanged();
}

void TurboSceneText::Scale(TurboVector2D scale)
{
	if (_scale.X == scale.X &&
		_scale.Y == scale.Y)
	{
		return;
	}

	_scale = scale;
	NotifyPropertyChanged();
}
