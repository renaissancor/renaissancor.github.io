#include <stdio.h>
#include <stdlib.h>

struct tNode
{
	int		Data;
	tNode* pNext;
};

// A container data type that stores data in a linked list structure
struct tList
{
	tNode* pHead; // Pointer to the node containing the very first data
	int    Size;   // Total number of data contained in the container
};

void PushBack(tList* _List, int _Data);

void PushFront(tList* _List, int _Data);

void Release(tList* _List);


int main() {
	tList myList;
	myList.pHead = nullptr;
	myList.Size = 0;

	// Test PushBack
	PushBack(&myList, 10);
	PushBack(&myList, 20);
	PushBack(&myList, 30);

	// Test PushFront
	PushFront(&myList, 5);
	PushFront(&myList, 1);

	// Print the list to verify PushBack and PushFront
	printf("List contents after PushFront and PushBack:\n");
	tNode* current = myList.pHead;
	while (current != nullptr) {
		printf("%d -> ", current->Data);
		current = current->pNext;
	}
	printf("NULL\n");

	// Print size
	printf("List size: %d\n", myList.Size);

	// Test Release
	Release(&myList);

	// Confirm list is released
	if (myList.pHead == nullptr && myList.Size == 0) {
		printf("List successfully released.\n");
	}
	else {
		printf("List release failed.\n");
	}

	return 0;
}




void PushBack(tList* _List, int _Data)
{
	tNode* pNode = (tNode*)malloc(sizeof(tNode));
	pNode->Data = _Data;
	pNode->pNext = nullptr;

	if (nullptr == _List->pHead)
	{
		_List->pHead = pNode;
	}

	else
	{
		tNode* pLastNode = _List->pHead;

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
	tNode* pNode = (tNode*)malloc(sizeof(tNode)); 
	pNode->Data = _Data; 
	pNode->pNext = _List->pHead; 
	_List->pHead = pNode; 
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

