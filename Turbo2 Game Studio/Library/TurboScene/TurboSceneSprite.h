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
			TurboSceneSprite(std::shared_ptr<ITurboSceneTexture> texture = nullptr);
			virtual ~TurboSceneSprite() {}

			//  ITurboSceneSprite Properties -----------------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboSceneTexture> Texture() { return _texture; }
			virtual void Texture(std::shared_ptr<ITurboSceneTexture> texture) { _texture = texture; }

			virtual TurboColor Color() { return _color; }
			virtual void Color(TurboColor color) { _color = color; }

			virtual bool UseRectangle() { return _useRectangle; }
			virtual void UseRectangle(bool useRectangle) { _useRectangle = useRectangle; }

			virtual TurboRectangle Rectangle() { return _rectangle; }
			virtual void Rectangle(TurboRectangle rectangle) { _rectangle = rectangle; }

			virtual TurboVector2D Location() { return _location; }
			virtual void Location(TurboVector2D location) { _location = location; }

			virtual TurboVector2D Origin() { return _origin; }
			virtual void Origin(TurboVector2D origin) { _origin = origin; }

			virtual float Rotation() { return _rotation; }
			virtual void Rotation(float rotation) { _rotation = rotation; }

			virtual TurboVector2D Scale() { return _scale; }
			virtual void Scale(TurboVector2D scale) { _scale = scale; }

		private:
			std::shared_ptr<ITurboSceneTexture> _texture;
			TurboColor _color {1, 1, 1, 1};

			bool _useRectangle = true;
			TurboRectangle _rectangle {};

			TurboVector2D _location {};
			TurboVector2D _origin {};
			float _rotation = 0;
			TurboVector2D _scale {1, 1};
		};
	}
}
