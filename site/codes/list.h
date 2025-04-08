#pragma once

struct tNode
{
	int		Data;
	tNode*	pNext;
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
