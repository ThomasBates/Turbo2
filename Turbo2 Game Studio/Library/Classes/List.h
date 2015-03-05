
#pragma once

#include "IList.h"

class List : public IList
{
private:
	void **_list;
	int _capacity;
	int _count;

protected:
	//  Local Support Methods
	void Grow();
	void QuickSort(void **sortList, int L, int R, ListSortCompare *compare);
	void Notify(void *ptr, ListNotification action);

public:
	//  Constructors and Destructors
	List();
	~List();

	//  IList Properties
	virtual int GetCapacity() { return _capacity; }
	virtual void SetCapacity(int value);

	virtual int GetCount() { return _count; }
	virtual void SetCount(int value);

	virtual void *GetItem(int index);
	virtual void SetItem(int index, void *item);

	virtual void **GetList() { return _list; }

	//	IList Methods
	virtual int Add(void *item);
	virtual void Clear();
	virtual void Delete(int index);
	virtual void Exchange(int index1, int index2);
	virtual IList *Expand();
	virtual void *Extract(void *item);
	virtual void *First();
	virtual int IndexOf(void *item);
	virtual int Contains(void *item);
	virtual void Insert(int index, void *item);
	virtual void *Last();
	virtual void Move(int curIndex, int newIndex);
	virtual int Remove(void *item);
	virtual void Pack();
	virtual void Sort(ListSortCompare *compare);
};
