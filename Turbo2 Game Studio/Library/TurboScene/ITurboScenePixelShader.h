#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Scene
	{
		class ITurboScenePixelShader
		{
		public:
			//  ITurboScenePixelShader Properties ----------------------------------------------------------------------
			virtual std::string Name() = 0;
		};
	}
}