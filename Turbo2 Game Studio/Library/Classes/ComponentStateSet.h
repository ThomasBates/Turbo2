
#pragma once

#include "List.h"

enum ComponentState
{
	csLoading, csReading, csWriting, csDestroying,
    csDesigning, csAncestor, csUpdating, csFixups, csFreeNotification,
	csInline, csDesignInstance
};

class ComponentStateSet
{
private:
	IList *_list;

public:
	ComponentStateSet();
	~ComponentStateSet();

	void Include(ComponentState state);
	void Exclude(ComponentState state);
	int Contains(ComponentState state);
};