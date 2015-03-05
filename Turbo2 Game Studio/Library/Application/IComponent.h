
#pragma once

#include "ComponentStateSet.h"
#include "ComponentStyleSet.h"

class IComponent
{
public:
	//	IComponent Properties
	virtual IComponent *GetComponent(int index) = 0;
	virtual int GetComponentCount() = 0;
	virtual int GetComponentIndex() = 0;
	virtual void SetComponentIndex(int value) = 0;
	virtual ComponentStateSet *GetComponentState() = 0;
	virtual ComponentStyleSet *GetComponentStyle() = 0;
	virtual IComponent *GetOwner() = 0;
	virtual char *GetName() = 0;
	virtual void SetName(char *value) = 0;
	virtual long GetTag() = 0;
	virtual void SetTag(long value) = 0;

	//	IComponent Methods
	virtual void BeforeDestruction() = 0;
	virtual void DestroyComponents() = 0;
	virtual void Destroying() = 0;
	virtual IComponent *FindComponent(char *name) = 0;
	virtual void FreeNotification(IComponent *component) = 0;
	virtual void RemoveFreeNotification(IComponent *component) = 0;
	virtual IComponent *GetParentComponent() = 0;
	virtual char *GetNamePath() = 0;
	virtual int HasParent() = 0;
	virtual void InsertComponent(IComponent *component) = 0;
	virtual void RemoveComponent(IComponent *component) = 0;
};

