
#pragma once

#include <pch.h>

#include <TurboView.h>
#include <ITurboGroupView.h>
#include <ITurboViewRendererAccess.h>

using namespace Turbo::Math;

namespace Turbo
{
	namespace View
	{
		class TurboGroupView : public TurboView, public ITurboGroupView
		{
		public:
			//	Constructors and Destructors ---------------------------------------------------------------------------
			TurboGroupView(std::string name,
						   std::shared_ptr<ITurboViewRendererAccess> rendererAccess);
			virtual ~TurboGroupView(){}

			//	ITurboView Methods -------------------------------------------------------------------------------------
			virtual void Load();
			virtual void Render();

			//	ITurboView Properties ----------------------------------------------------------------------------------
			virtual void IsVisible(bool isVisible);

			//	ITurboGroupView Properties -----------------------------------------------------------------------------
			virtual std::vector<std::shared_ptr<ITurboView>> Views() { return _views; }

			//	ITurboGroupView Methods --------------------------------------------------------------------------------
			virtual void AddView(std::shared_ptr<ITurboView> view);

		protected:
			void InternalAddView(const std::shared_ptr<ITurboView>& view);

		private:
			std::vector<std::shared_ptr<ITurboView>> _views;
		};
	}
}