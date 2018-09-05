#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneFont
		{
		public:
			//  ITurboSceneFont Properties -----------------------------------------------------------------------------
			virtual std::string Name() = 0;
		};
	}
}