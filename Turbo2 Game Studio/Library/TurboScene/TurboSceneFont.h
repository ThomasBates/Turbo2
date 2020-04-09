#pragma once

#include <pch.h>

#include <ITurboSceneFont.h>
#include <TurboRectangle.h>

using namespace Turbo::Graphics;

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneFont : public ITurboSceneFont
		{
		public:
			//  Constructors -------------------------------------------------------------------------------------------
			TurboSceneFont(std::string name);
			virtual ~TurboSceneFont() {}

			//  ITurboSceneFont Properties -----------------------------------------------------------------------------
			virtual std::string Name() { return _name; }

			//  ITurboSceneFont Methods --------------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboSceneMesh> CreateMesh(std::shared_ptr<ITurboSceneText> sceneText);

        protected:
            void SetTextureExtent(float width, float height);
			void SetCharacterExtent(char character, short left, short top, short right, short bottom);

		private:
			std::string _name;

			TurboVector2D _textureSize;
            std::map<char, TurboRectangle> _characterExtent;
		};
	}
}