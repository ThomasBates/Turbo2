
#include <stdlib.h>
#include <string.h>
#include "List.h"

const int MaxListSize = INT_MAX / 16;


//  ============================================================================
//              List
//  ============================================================================

//============  Constructors and Destructors  --------------------------------------------------------------------------

List::List()
{
}

List::~List()
{
	Clear();
}

//----------------------------------------------------------------------------------------------------------------------
//============  IList Properties  --------------------------------------------------------------------------------------

void List::SetCapacity(int newCapacity)
{
	if (newCapacity < _count || newCapacity > MaxListSize)
//		Error(SListCapacityError, newCapacity);
		return;

	if (newCapacity != _capacity)
	{
		_list = (void**)realloc(_list, newCapacity * sizeof(void*));
		_capacity = newCapacity;
	}
}

void List::SetCount(int newCount)
{
	if (newCount < 0 || newCount > MaxListSize)
//		Error(SListCountError, newCapacity);
		return;

	if (newCount > _capacity)
		SetCapacity(newCount);
	if (newCount > _count)
		memset(&_list[_count], 0, (newCount - _count) * sizeof(void*));
	else
		for (int i = _count - 1; i >= newCount; i-- )
			Delete(i);
	_count = newCount;
}

void *List::GetItem(int index)
{
	if ((index < 0) || (index >= _count))
//		Error(SListIndexError, index);
		return 0;

	return _list[index];
}

void List::SetItem(int index, void *item)
{
	if ((index < 0) || (index >= _count))
//		Error(SListIndexError, index);
		return;

	void *temp = _list[index];
	_list[index] = item;
	if (temp != 0)
		Notify(temp, lnDeleted);
	if (item != 0)
		Notify(item, lnAdded);
}

//----------------------------------------------------------------------------------------------------------------------
//============  IList Methods  -----------------------------------------------------------------------------------------

int List::Add(void *item)
{
	int result = _count;
	if (result == _capacity)
		Grow();
	_list[result] = item;
	_count++;
	if (item != 0)
		Notify(item, lnAdded);
	return result;
}

void List::Clear()
{
	SetCount(0);
	SetCapacity(0);
}

void List::Delete(int index)
{
	if ((index < 0) || (index >= _count))
//		Error(SListIndexError, index);
		return;

	void *temp = GetItem(index);
	_count--;
	if (index < _count)
		memmove(_list[index], _list[index+1], (_count - index) * sizeof(void*));
	if (temp != 0)
		Notify(temp, lnDeleted);
}

void List::Exchange(int index1, int index2)
{
	if ((index1 < 0) || (index1 >= _count))
//		Error(SListIndexError, index1);
		return;

	if ((index2 < 0) || (index2 >= _count))
//		Error(SListIndexError, index2);
		return;

	void *item = _list[index1];
	_list[index1] = _list[index2];
	_list[index2] = item;
}

IList *List::Expand()
{
	if (_count == _capacity)
		Grow();
	return this;
}

void *List::First()
{
	return GetItem(0);
}

int List::IndexOf(void *item)
{
	int result = 0;
	while ((result < _count) && (_list[result] != item))
		result++;
	if (result == _count)
		result = -1;
	return result;
}

int List::Contains(void *item)
{
	return IndexOf(item) >= 0;
}

void List::Insert(int index, void *item)
{
	if ((index < 0) || (index >= _count))
//		Error(SListIndexError, index);
		return;

	if (_count == _capacity)
		Grow();
	if (index < _count)
		memmove(_list[index+1], _list[index], (_count - index) * sizeof(void*));
	_list[index] = item;
	_count++;
	if (item != 0)
		Notify(item, lnAdded);
}

void *List::Last()
{
	return GetItem(_count - 1);
}

void List::Move(int curIndex, int newIndex)
{
	if (curIndex != newIndex)
	{
		if ((newIndex < 0) || (newIndex >= _count))
//			Error(SListIndexError, newIndex);
			return;

		void *item = GetItem(curIndex);
		_list[curIndex] = 0;
		Delete(curIndex);
		Insert(newIndex, 0);
		_list[newIndex] = item;
	}
}

int List::Remove(void *item)
{
	int result = IndexOf(item);
	if (result >= 0)
		Delete(result);
	return result;
}

void List::Pack()
{
	for (int i=_count-1; i >= 0; i--)
		if (GetItem(i) == 0)
			Delete(i);
}

void List::Sort(ListSortCompare *compare)
{
	if (_list != 0 && _count != 0)
		QuickSort(_list, 0, _count - 1, compare);
}

void *List::Extract(void *item)
{
	void *result = 0;
	int I = IndexOf(item);
	if (I >= 0)
	{
		result = item;
		_list[I] = 0;
		Delete(I);
		Notify(result, lnExtracted);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//============  Local Support Methods  ---------------------------------------------------------------------------------

void List::Grow()
{
	int delta;

	if (_capacity > 64)
		delta = _capacity / 4;
	else if (_capacity > 8)
		delta = 16;
	else
		delta = 4;

	SetCapacity(_capacity + delta);
}

void List::QuickSort(void **sortList, int L, int R, ListSortCompare *compare)
{
	int I, J;
	void *P, *T;
	do
	{
		I = L;
		J = R;
		P = sortList[(L+R)>>1];
		do
		{
			while (compare(sortList[I], P) < 0)
				I++;
			while (compare(sortList[J], P) > 0)
				J--;
			if (I <= J)
			{
				T = sortList[I];
				sortList[I] = sortList[J];
				sortList[J] = T;
				I++;
				J--;
			}
		} while (I <= J);
		if (L < J)
			QuickSort(sortList, L, J, compare);
		L = I;
	} while (I < R);
}

void List::Notify(void *ptr, ListNotification action)
{
}

//----------------------------------------------------------------------------------------------------------------------
