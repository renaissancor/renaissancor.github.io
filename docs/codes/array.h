#pragma once

// Dynamic Array (Resizable Array)
struct tArray
{
	int* pArr;     // Address of storage space
	int   Capacity; // Capacity
	int   Size;     // Current size
};

void InitArr(tArray* _pArr);

void PushBack(tArray* _pArr, int _Data);

