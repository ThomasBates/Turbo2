
#pragma once

#include <pch.h>

#include <TurboVector2D.h>

using namespace Turbo::Math;

namespace Turbo
{
	namespace View
	{
		class ITurboView
		{
		public:
			//	ITurboView Properties ----------------------------------------------------------------------------------
			virtual std::string Name() = 0;

			virtual TurboVector2D Position() = 0;
			virtual void Position(TurboVector2D position) = 0;

			virtual TurboVector2D Size() = 0 ;
			virtual void Size(TurboVector2D size) = 0;

			//	ITurboView Methods -------------------------------------------------------------------------------------
			virtual bool Contains(float x, float y) = 0;
			virtual void Load() = 0;
			virtual void Render() = 0;
		};
	}
}