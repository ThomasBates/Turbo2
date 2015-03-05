
#pragma once

#include "List.h"

enum ComponentStyle
{
	csInheritable, csCheckPropAvail
};

class ComponentStyleSet
{
private:
	IList *_list;

public:
	ComponentStyleSet();
	~ComponentStyleSet();

	void Include(ComponentStyle style);
	void Exclude(ComponentStyle style);
	int Contains(ComponentStyle style);
};