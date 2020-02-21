
#include <pch.h>

#include <TurboGroupView.h>


using namespace Turbo::View;

//	Constructors and Destructors ---------------------------------------------------------------------------------------

TurboGroupView::TurboGroupView(std::string name) :
	TurboView(name)
{
}

//	ITurboView Methods ------------------------------------------------------------------------------------------

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

//	ITurboViewGroupControl Methods -------------------------------------------------------------------------------------

void TurboGroupView::AddView(std::shared_ptr<ITurboView> view)
{
	_views.push_back(view);
}
