
#pragma once

#include "IComponent.h"
#include "List.h"

enum Operation 
{
	opInsert, opRemove
};

typedef void GetChildProc(IComponent*);

class Component : public IComponent
{
private:
	//	Private Members
	IComponent	*_owner;
	char		*_name;
	long		 _tag;
	IList		*_components;
	IList		*_freeNotifies;
	long		 _designInfo;
	ComponentStateSet	*_componentState;

	//	Local Support Methods
	void Insert(IComponent *component);
	void Remove(IComponent *component);
	void RemoveNotification(IComponent *component);
//	void SetReference(int Enable);

protected:
	ComponentStyleSet	*_componentStyle;

	//	Local Support Methods
	void ChangeName(const char *newName);
	virtual void GetChildren(GetChildProc *proc, IComponent *root);
	virtual IComponent *GetChildOwner();
	virtual IComponent *GetChildParent();
	virtual void Loaded();
	virtual void Notification(IComponent *component, Operation operation);
	void SetAncestor(int value);
	void SetDesigning(int value, int setChildren);
	void SetInline(int value);
	void SetDesignInstance(int value);
	virtual void SetChildOrder(IComponent *child, int order);
	virtual void SetParentComponent(IComponent *value);
	virtual void Updating();
	virtual void Updated();
	virtual void ValidateRename(IComponent *component, const char *curName, const char *newName);
	virtual void ValidateContainer(IComponent *component);
	virtual void ValidateInsert(IComponent *component);

public:
	//	Constructors and Destructors
	Component(IComponent *owner);
	~Component();

	//	IComponent Properties
	virtual IComponent *GetComponent(int index);
	virtual int GetComponentCount();
	virtual int GetComponentIndex();
	virtual void SetComponentIndex(int value);
	virtual ComponentStateSet *GetComponentState() { return _componentState; }
	virtual ComponentStyleSet *GetComponentStyle() { return _componentStyle; }
	virtual IComponent *GetOwner() { return _owner; }
	virtual char *GetName() { return _name; }
	virtual void SetName(const char *newName);
	virtual long GetTag() { return _tag; }
	virtual void SetTag(long value) { _tag = value; }

	//  Properties
	virtual void SetOwner(IComponent *value) { _owner = value; }

	//	IComponent Methods
	virtual void BeforeDestruction();
	virtual void DestroyComponents();
	virtual void Destroying();
	virtual IComponent *FindComponent(const char *name);
	virtual void FreeNotification(IComponent *component);
	virtual void RemoveFreeNotification(IComponent *component);
	virtual IComponent *GetParentComponent();
	virtual char *GetNamePath();
	virtual int HasParent();
	virtual void InsertComponent(IComponent *component);
	virtual void RemoveComponent(IComponent *component);

	//  Public Access Methods


};