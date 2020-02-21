#pragma once

#include <pch.h>

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
			virtual ~TurboScenePixelShader() {};

			//  ITurboScenePixelShader Properties ------------------------------------------------------------------------------
			virtual std::string Name() { return _name; }

		private:
			std::string _name;
		};
	}
}