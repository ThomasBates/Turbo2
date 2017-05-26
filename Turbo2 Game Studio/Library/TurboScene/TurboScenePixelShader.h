#pragma once

#include <ITurboScenePixelShader.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboScenePixelShader : public ITurboScenePixelShader
		{
		public:
			//  Constructors ---------------------------------------------------------------------------------------------------
			TurboScenePixelShader(std::string name);

			//  ITurboScenePixelShader Properties ------------------------------------------------------------------------------
			virtual std::string Name() { return _name; }

		private:
			std::string _name;
		};
	}
}