
#include <string.h>

#include "Component.h"

//  ============================================================================
//              Component
//  ============================================================================

//============  Constructors and Destructors  --------------------------------------------------------------------------

Component::Component(IComponent *owner)
{
	//_componentStyle->Set(csInheritable);
	if (owner != 0)
		owner->InsertComponent(this);
}

Component::~Component()
{
	Destroying();
	if (_freeNotifies != 0)
	{
		for (int i = _freeNotifies->GetCount() - 1; i >= 0; i-- )
		{
			((Component*)_freeNotifies->GetItem(i))->Notification(this, opRemove);
			if (_freeNotifies == 0)
				break;
		}
		delete _freeNotifies;
		_freeNotifies = 0;
	}
	DestroyComponents();
	if (_owner != 0)
		_owner->RemoveComponent(this);
}

//----------------------------------------------------------------------------------------------------------------------
//============  IComponent Properties  ---------------------------------------------------------------------------------

IComponent *Component::GetComponent(int index)
{
	if (_components == 0)
	{
		//throw new ListIndexError(index);
		return 0;
	}
	return (IComponent*)_components->GetItem(index);
}

int Component::GetComponentCount()
{
	if (_components != 0)
		return _components->GetCount();
	return 0;
}

int Component::GetComponentIndex()
{
	if (_owner != 0 && ((Component*)_owner)->_components != 0)
		return ((Component*)_owner)->_components->IndexOf(this);
	return -1;
}

void Component::SetComponentIndex(int value)
{
	if (_owner != 0)
	{
		IList *ownerComponents = ((Component*)_owner)->_components;

		int i = ownerComponents->IndexOf(this);
		if (i >= 0)
		{
			int count = ownerComponents->GetCount();
			if (value < 0) value = 0;
			if (value >= count) value = count - 1;
			if (value != i)
			{
				ownerComponents->Delete(i);
				ownerComponents->Insert(value, this);
			}
		}
	}
}

void Component::SetName(const char *newName)
{
	if (strcmp(_name, newName) != 0)
	{
		//if (strlen(newName) != 0 && !IsValidIdent(newName))
		//{
		//	//throw new EComponentError("Invalid Name", newName);
		//	return;
		//}

		if (_owner != 0)
			((Component*)_owner)->ValidateRename(this, _name, newName);
		else
			ValidateRename(0, _name, newName);

		//SetReference(0);
		ChangeName(newName);
		//SetReference(1);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//============  IComponent Methods  ------------------------------------------------------------------------------------

void Component::BeforeDestruction()
{
	if (!_componentState->Contains(csDestroying))
		Destroying();
}

void Component::DestroyComponents()
{
	while (_components != 0)
	{
		IComponent *instance = (IComponent*)_components->Last();

		if (instance->GetComponentState()->Contains(csFreeNotification) ||
			(_componentState->Contains(csDesigning) && _componentState->Contains(csInline)))
			RemoveComponent(instance);
		else
			Remove(instance);

		delete instance;
	}
}

void Component::Destroying()
{
	if (!_componentState->Contains(csDestroying))
	{
		_componentState->Include(csDestroying);

		if (_components != 0)
			for (int i = 0; i < _components->GetCount(); i++)
				((IComponent*)_components->GetItem(i))->Destroying();
	}
}

IComponent *Component::FindComponent(const char *name)
{
	if (strlen(_name) != 0 && _components != 0)
	{
		for (int i=0; i<_components->GetCount(); i++)
		{
			IComponent *component = (IComponent*)_components->GetItem(i);
			if (stricmp(component->GetName(), name) == 0)
				return component;
		}
	}
	return 0;
}

void Component::FreeNotification(IComponent *component)
{
	if (_owner == 0 || component->GetOwner() != _owner)
	{
		if (_freeNotifies == 0) _freeNotifies = new List;
		if (!_freeNotifies->Contains(component))
		{
			_freeNotifies->Add(component);
			component->FreeNotification(this);
		}
	}
	_componentState->Include(csFreeNotification);
}

void Component::RemoveFreeNotification(IComponent *component)
{
  RemoveNotification(component);
  ((Component*)component)->RemoveNotification(this);
}

IComponent *Component::GetParentComponent()
{
	return 0;
}

char *Component::GetNamePath()
{
	return _name;
}

int Component::HasParent()
{
	return 0;
}

void Component::InsertComponent(IComponent *component)
{
	((Component*)component)->ValidateContainer(this);
	ValidateRename(component, "", component->GetName());
	Insert(component);
	//component->SetReference(1);
	if (_componentState->Contains(csDesigning))
		((Component*)component)->SetDesigning(1,1);
	Notification(component, opInsert);
}

void Component::RemoveComponent(IComponent *component)
{
	ValidateRename(component, component->GetName(), "");
	Notification(component, opRemove);
	//((Component*)component)->SetReference(0);
	Remove(component);
}

//----------------------------------------------------------------------------------------------------------------------
//============  Public Access Methods  ---------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods  ---------------------------------------------------------------------------------

void Component::Insert(IComponent *component)
{
	if (_components == 0)
		_components = new List;
	_components->Add(component);
	((Component*)component)->SetOwner(this);
}

void Component::Remove(IComponent *component)
{
	((Component*)component)->SetOwner(0);
	_components->Remove(component);
	if (_components->GetCount() == 0)
	{
		delete _components;
		_components = 0;
	}
}

void Component::RemoveNotification(IComponent *component)
{
	if (_freeNotifies != 0)
	{
		_freeNotifies->Remove(component);
		if (_freeNotifies->GetCount() == 0)
		{
			delete _freeNotifies;
			_freeNotifies = 0;
		}
	}
}

//void Component::SetReference(int Enable){}

void Component::ChangeName(const char *newName)
{
	if (strcmp(_name, newName) != 0)
	{
		if (_name != 0)
			delete [] _name;
		_name = new char[strlen(newName)+1];
		strcpy(_name, newName);
	}
}

void Component::GetChildren(GetChildProc *proc, IComponent *root)
{
}

IComponent *Component::GetChildOwner()
{
	return 0;
}

IComponent *Component::GetChildParent()
{
	return this;
}

void Component::Loaded()
{
	_componentState->Exclude(csLoading);
}

void Component::Notification(IComponent *component, Operation operation)
{
	if (operation == opRemove && component != 0)
		RemoveFreeNotification(component);

	if (_components != 0)
		for (int i=0; i<_components->GetCount(); i++)
			((Component*)_components->GetItem(i))->Notification(component, operation);
}

void Component::SetAncestor(int value)
{
	if (value)
		_componentState->Include(csAncestor);
	else
		_componentState->Exclude(csAncestor);

	for (int i=0; i<GetComponentCount(); i++)
		((Component*)_components->GetItem(i))->SetAncestor(value);
}

void Component::SetDesigning(int value, int setChildren)
{
	if (value)
		_componentState->Include(csDesigning);
	else
		_componentState->Exclude(csDesigning);

	if (setChildren)
		for (int i=0; i<GetComponentCount(); i++)
			((Component*)_components->GetItem(i))->SetDesigning(value, setChildren);
}

void Component::SetInline(int value)
{
	if (value)
		_componentState->Include(csInline);
	else
		_componentState->Exclude(csInline);
}

void Component::SetDesignInstance(int value)
{
	if (value)
		_componentState->Include(csDesignInstance);
	else
		_componentState->Exclude(csDesignInstance);
}

void Component::SetChildOrder(IComponent *child, int order)
{
}

void Component::SetParentComponent(IComponent *value)
{
}

void Component::Updating()
{
	_componentState->Include(csUpdating);
}

void Component::Updated()
{
	_componentState->Exclude(csUpdating);
}

void Component::ValidateRename(IComponent *component, const char *curName, const char *newName)
{
	if (component != 0 && stricmp(curName, newName) != 0 &&
		component->GetOwner() != this && FindComponent(newName) != 0)
	{
		//throw(new EComponentException("Duplicate Name", newName);
		return;
	}	

	if (_componentState->Contains(csDesigning) && _owner != 0)
		((Component*)_owner)->ValidateRename(component, curName, newName);
}

void Component::ValidateContainer(IComponent *component)
{
  ((Component*)component)->ValidateInsert(this);
}

void Component::ValidateInsert(IComponent *component)
{
}

//----------------------------------------------------------------------------------------------------------------------
