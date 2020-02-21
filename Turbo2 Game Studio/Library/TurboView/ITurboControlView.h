
#pragma once

#include <pch.h>

#include <ITurboView.h>
#include <ITurboControlViewModel.h>

using namespace Turbo::View;

namespace Turbo
{
	namespace View
	{
		class ITurboControlView : public virtual ITurboView
		{
		public:
			//	ITurboControlView Properties ---------------------------------------------------------------------------
			virtual bool IsActive() = 0;
			virtual void IsActive(bool isActive) = 0;

            virtual std::shared_ptr<ITurboControlViewModel> ControlViewModel() = 0;

			//	ITurboControlView Methods ------------------------------------------------------------------------------
			virtual void CurrentPoint(float x, float y, float z = 0) = 0;
		};
	}
}