
#include "ComponentStateSet.h"

ComponentStateSet::ComponentStateSet()
{
	_list = new List();
}

ComponentStateSet::~ComponentStateSet()
{
	delete _list;
}

void ComponentStateSet::Include(ComponentState state)
{
	if (!Contains(state))
		_list->Add((void*)state);
}

void ComponentStateSet::Exclude(ComponentState state)
{
	if (Contains(state))
		_list->Remove((void*)state);
}

int ComponentStateSet::Contains(ComponentState state)
{
	return _list->Contains((void*)state);
}

