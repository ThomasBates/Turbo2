
#pragma once

#include <ITurboViewRendererAccess.h>

using namespace Turbo::View;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameRenderer
		{
		public:
			//	ITurboGameRenderer Properties --------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboViewRendererAccess> RendererAccess() = 0;

			//	ITurboGameRenderer Methods -----------------------------------------------------------------------------
			virtual void InitializeLoading() = 0;
			virtual void FinalizeLoading() = 0;
			virtual void InitializeRendering() = 0;
			virtual void FinalizeRendering() = 0;
			virtual void Reset() = 0;
		};
	}
}
