
#pragma once

#include <ITurboView.h>

using namespace Turbo::View;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameRenderer
		{
		public:
			//	ITurboGameRenderer Methods -----------------------------------------------------------------------------
			virtual void UpdateDisplayInformation() = 0;
			virtual bool LoadView(std::shared_ptr<ITurboView> view) = 0;
			virtual bool RenderView(std::shared_ptr<ITurboView> view) = 0;
			virtual void Reset() = 0;
		};
	}
}
