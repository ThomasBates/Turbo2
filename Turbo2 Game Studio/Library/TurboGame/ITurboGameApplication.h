
#pragma once

#include <pch.h>

#include <ITurboGame.h>
#include <ITurboView.h>

using namespace Turbo::View;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameApplication
		{
		public:
			//	ITurboGameApplication Methods --------------------------------------------------------------------------
			virtual int Run(std::shared_ptr<ITurboGame> game, std::shared_ptr<ITurboView> view) = 0;
		};
	}
}