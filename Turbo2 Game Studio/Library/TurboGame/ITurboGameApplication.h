
#pragma once

#include <pch.h>

#include <ITurboGame.h>

namespace Turbo
{
	namespace Game
	{
		class ITurboGameApplication
		{
		public:
			virtual int Run(std::shared_ptr<ITurboGame> game) = 0;
		};
	}
}