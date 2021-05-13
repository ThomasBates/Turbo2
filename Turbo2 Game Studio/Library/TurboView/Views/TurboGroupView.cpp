
#include <pch.h>

#include <TurboGroupView.h>

using namespace Turbo::View;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

TurboGroupView::TurboGroupView(std::string name,
							   std::shared_ptr<ITurboViewRendererAccess> rendererAccess) :
	TurboView(name, rendererAccess)
{
}

//	ITurboView Methods -------------------------------------------------------------------------------------------------

void TurboGroupView::Load()
{
	for (auto& view : _views)
		if (view->IsVisible())
			view->Load();
}

void TurboGroupView::Render()
{
	for (auto& view : _views)
		if (view->IsVisible())
			view->Render();
}

//	ITurboView Properties ----------------------------------------------------------------------------------------------

void TurboGroupView::IsVisible(bool isVisible)
{
	TurboView::IsVisible(isVisible);

	for (auto& view : _views)
		view->IsVisible(isVisible);
}

//	ITurboGroupView Methods --------------------------------------------------------------------------------------------

void TurboGroupView::AddView(std::shared_ptr<ITurboView> view)
{
	InternalAddView(view);
}

//	Protected Methods --------------------------------------------------------------------------------------------------

void TurboGroupView::InternalAddView(const std::shared_ptr<ITurboView>& view)
{
	_views.push_back(view);
}
