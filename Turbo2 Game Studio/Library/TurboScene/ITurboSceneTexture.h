#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneTexture
		{
		public:
			//  ITurboSceneTexture Properties --------------------------------------------------------------------------
			virtual std::string Name() = 0;
		};
	}
}