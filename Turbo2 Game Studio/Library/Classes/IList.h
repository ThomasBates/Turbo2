
#pragma once

typedef int ListSortCompare(void*, void*);
enum ListNotification {lnAdded, lnExtracted, lnDeleted};

class IList
{
public:
	//	IList Methods
	virtual int Add(void *item) = 0;
	virtual void Clear() = 0;
	virtual void Delete(int index) = 0;
	virtual void Exchange(int index1, int index2) = 0;
	virtual IList *Expand() = 0;
	virtual void *Extract(void *item) = 0;
	virtual void *First() = 0;
	virtual int IndexOf(void *item) = 0;
	virtual int Contains(void *item) = 0;
	virtual void Insert(int index, void *item) = 0;
	virtual void *Last() = 0;
	virtual void Move(int curIndex, int newIndex) = 0;
	virtual int Remove(void *item) = 0;
	virtual void Pack() = 0;
	virtual void Sort(ListSortCompare *compare) = 0;

	//  IList Properties
	virtual int GetCapacity() = 0;
	virtual void SetCapacity(int value) = 0;
	virtual int GetCount() = 0;
	virtual void SetCount(int value) = 0;
	virtual void *GetItem(int index) = 0;
	virtual void SetItem(int index, void *item) = 0;
	virtual void **GetList() = 0;
};
