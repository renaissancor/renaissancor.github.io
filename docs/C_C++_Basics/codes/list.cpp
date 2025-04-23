
#include "list.h"
#include <stdlib.h>

void PushBack(tList* _List, int _Data)
{
	// Create a new node to store the input data
	tNode* pNode = (tNode*)malloc(sizeof(tNode));
	pNode->Data = _Data;
	pNode->pNext = nullptr;

	// No data has been input into the container yet
	// Case for first data input
	if (nullptr == _List->pHead)
	{
		// List container points to the first node
		_List->pHead = pNode;
	}

	// From the second data input onward
	else
	{
		// Find the last of the data (nodes) owned by the list
		tNode* pLastNode = _List->pHead;

		// Advance to the next node until the Next pointer in the current node points to nullptr
		while (nullptr != pLastNode->pNext)
		{
			pLastNode = pLastNode->pNext;
		}

		/* Alternative loop
		for (int i = 0; i < _List->Size - 1; ++i)
		{
			pLastNode = pLastNode->pNext;
		}*/

		// Connect the found last node to the newly created node
		pLastNode->pNext = pNode;
	}

	++_List->Size;
}

void PushFront(tList* _List, int _Data)
{
	// Dynamically allocate space (node) to store the data
	tNode* pNode = (tNode*)malloc(sizeof(tNode));

	// Store the data in the node created in the heap
	pNode->Data = _Data;

	// Link the node currently pointed to by the list as the next node
	pNode->pNext = _List->pHead;

	// The list now points to the newly created node as the first node
	_List->pHead = pNode;

	// Increase data count
	++(_List->Size);
}

void Release(tList* _List)
{
	tNode* pNode = _List->pHead;

	// Clean up memory for all nodes
	//for (int i = 0; i < _List->Size; ++i)
	//{
	//	tNode* pNext = pNode->pNext;
	//	free(pNode);
	//	pNode = pNext;
	//}

	while (nullptr != pNode)
	{
		tNode* pNext = pNode->pNext;
		free(pNode);
		pNode = pNext;
	}

	_List->pHead = nullptr;
	_List->Size = 0;
}