#include "array.h"
#include <stdlib.h>

void InitArr(tArray* _pArr)
{
	_pArr->Capacity = 4;
	_pArr->Size = 0;
	_pArr->pArr = (int*)malloc(sizeof(int) * _pArr->Capacity);
}

void PushBack(tArray* _pArr, int _Data)
{
	// The dynamic array object is full
	if (_pArr->Size == _pArr->Capacity)
	{
		// Allocate additional memory
		int* pNew = (int*)malloc(sizeof(int) * _pArr->Capacity * 2);

		// Move existing data to new memory
		for (int i = 0; i < _pArr->Size; ++i)
		{
			pNew[i] = _pArr->pArr[i];
		}

		// Free the memory where the old data was stored
		free(_pArr->pArr);

		// Let the dynamic array object point to the newly allocated space
		_pArr->pArr = pNew;

		// Update capacity
		_pArr->Capacity *= 2;
	}

	// Store the input data in the heap space managed by the dynamic array and increase the count
	_pArr->pArr[_pArr->Size++] = _Data;
}
