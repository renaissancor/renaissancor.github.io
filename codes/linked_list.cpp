#include "List.h"

#include <stdlib.h>
#include <stdio.h>

// Data Structure
// The study of handling data effectively

// Stack
// - Last In, First Out

// Queue
// - First In, First Out

// Dynamic Array
// - Stores data consecutively

// Linked List
// - Data is not stored consecutively, but each piece links to the next, allowing traversal

// Big O notation
// A symbol representing algorithm efficiency, assuming the worst case.
// O(1)      - Constant time, independent of input size
// O(LogN)   - Efficiency that divides input size by squares (e.g., binary search)
// O(N)      - Linear time proportional to the number of inputs
// O(N LogN) - Linearithmic time
// O(N^2)    - Quadratic time

//                    Data Indexing      Data Insertion                Data Traversal
// Dynamic Array          O(1)            O(N) - if inserted at front       O(N)
// Linked List            O(N)            O(1)                              O(N)

#include <crtdbg.h>

int main()
{
	// Memory leak check in debug mode
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	tList list = {};
	PushBack(&list, 100);
	PushBack(&list, 200);
	PushBack(&list, 300);

	tList list2 = {};
	PushFront(&list2, 100);
	PushFront(&list2, 200);
	PushFront(&list2, 300);

	// 300 200 100
	tNode* pNode = list2.pHead;
	for (int i = 0; i < list2.Size; ++i)
	{
		printf("%d\n", pNode->Data);
		pNode = pNode->pNext;
	}

	// Memory release
	Release(&list);
	Release(&list2);

	PushBack(&list, 1);
	Release(&list);

	return 0;
}
