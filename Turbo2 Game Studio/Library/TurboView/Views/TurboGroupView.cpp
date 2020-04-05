
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
		view->Load();
}

void TurboGroupView::Render()
{
	for (auto& view : _views)
		view->Render();
}

//	ITurboGroupView Methods --------------------------------------------------------------------------------------------

void TurboGroupView::AddView(std::shared_ptr<ITurboView> view)
{
	_views.push_back(view);
}
