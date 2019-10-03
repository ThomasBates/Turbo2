#pragma once

#include <ITurboSceneSprite.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneSprite : public ITurboSceneSprite
		{
		public:
			//  Constructors and Destructors -----------------------------------------------------------------------------------
			TurboSceneSprite();
			TurboSceneSprite(std::shared_ptr<ITurboSceneTexture> texture);
			virtual ~TurboSceneSprite() {}

			//  ITurboSceneSprite Properties -----------------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboSceneTexture> Texture() { return _texture; }
			virtual void Texture(std::shared_ptr<ITurboSceneTexture> texture) { _texture = texture; }

			virtual TurboColor Color() { return _color; }
			virtual void Color(TurboColor color) { _color = color; }

			virtual bool UseRectangle() { return _useRectangle; }
			virtual void UseRectangle(bool useRectangle) { _useRectangle = useRectangle; }

			virtual float Left() { return _left; }
			virtual void Left(float left) { _left = left; }

			virtual float Top() { return _top; }
			virtual void Top(float top) { _top = top; }

			virtual float Right() { return _right; }
			virtual void Right(float right) { _right = right; }

			virtual float Bottom() { return _bottom; }
			virtual void Bottom(float bottom) { _bottom = bottom; }

			virtual TurboVector2D Location() { return _location; }
			virtual void Location(TurboVector2D location) { _location = location; }

			virtual TurboVector2D Origin() { return _origin; }
			virtual void Origin(TurboVector2D origin) { _origin = origin; }

			virtual float Rotation() { return _rotation; }
			virtual void Rotation(float rotation) { _rotation = rotation; }

			virtual float Scale() { return _scale; }
			virtual void Scale(float scale) { _scale = scale; }

			//  ITurboSceneSprite Methods --------------------------------------------------------------------------------------
			virtual void Update(NavigationInfo navInfo) {}

		private:
			std::shared_ptr<ITurboSceneTexture>	_texture;
			TurboColor							_color = TurboColor(1.0f, 1.0f, 1.0f, 1.0f);
			bool								_useRectangle = true;
			float								_left = 0.0f;
			float								_top = 0.0f;
			float								_right = 1.0f;
			float								_bottom = 1.0f;
			TurboVector2D						_location = TurboVector2D(0.0f, 0.0f);
			TurboVector2D						_origin = TurboVector2D(0.0f, 0.0f);
			float								_rotation = 0.0f;
			float								_scale = 1.0f;
		};
	}
}
