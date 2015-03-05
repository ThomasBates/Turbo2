
#include "ComponentStyleSet.h"

ComponentStyleSet::ComponentStyleSet()
{
	_list = new List();
}

ComponentStyleSet::~ComponentStyleSet()
{
	delete _list;
}

void ComponentStyleSet::Include(ComponentStyle style)
{
	if (!Contains(style))
		_list->Add((void*)style);
}

void ComponentStyleSet::Exclude(ComponentStyle style)
{
	if (Contains(style))
		_list->Remove((void*)style);
}

int ComponentStyleSet::Contains(ComponentStyle style)
{
	return _list->Contains((void*)style);
}

