
#include <pch.h>

#include <TurboView.h>

using namespace Turbo::View;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

TurboView::TurboView(std::string name,
					 std::shared_ptr<ITurboViewRendererAccess> rendererAccess) :
	_name(name),
	_rendererAccess(rendererAccess)
{
}

//	ITurboView Properties ---------------------------------------------------------------------------------------

void TurboView::IsVisible(bool isVisible)
{
	if (_isVisible == isVisible)
	{
		return;
	}

	_isVisible = isVisible;

	if (_isVisible)
		UpdateLayout(_position, _size);
}

void TurboView::Position(TurboVector2D position)
{
	if (_position.X == position.X &&
		_position.Y == position.Y)
	{
		return;
	}

	_position = position;
	if (_isVisible)
		UpdateLayout(_position, _size);
}

void TurboView::Size(TurboVector2D size)
{
	if (_size.X == size.X &&
		_size.Y == size.Y)
	{
		return;
	}

	_size = size;
	if (_isVisible)
		UpdateLayout(_position, _size);
}

//	ITurboView Methods ------------------------------------------------------------------------------------------

bool TurboView::Contains(float x, float y)
{
	return _position.X <= x && x < _position.X + _size.X &&
		   _position.Y <= y && y < _position.Y + _size.Y;
}
