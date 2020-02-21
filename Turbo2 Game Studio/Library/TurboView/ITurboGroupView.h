
#pragma once

#include <pch.h>

#include <ITurboView.h>

namespace Turbo
{
	namespace View
	{
		class ITurboGroupView : public virtual ITurboView
		{
		public:
			//	ITurboViewGroupControl Properties ----------------------------------------------------------------------
			virtual std::vector<std::shared_ptr<ITurboView>> Views() = 0;

			//	ITurboViewGroupControl Methods -------------------------------------------------------------------------
			virtual void AddView(std::shared_ptr<ITurboView> view) = 0;
		};
	}
}