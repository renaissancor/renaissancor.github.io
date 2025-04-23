#include <stdlib.h>
#include <stdio.h>

#include "Array.h"

int main()
{
	// Array
	int iArr[10] = {};

	// Dynamic Array
	tArray arr = {};
	InitArr(&arr);

	PushBack(&arr, 100);
	PushBack(&arr, 200);
	PushBack(&arr, 300);
	PushBack(&arr, 400);
	PushBack(&arr, 500);
	PushBack(&arr, 600);
	PushBack(&arr, 700);
	PushBack(&arr, 800);
	PushBack(&arr, 900);

	for (int i = 0; i < arr.Size; ++i)
	{
		printf("%d\n", arr.pArr[i]);
	}

	// Heap Corruption
	// When accessing and modifying a space that has not been dynamically allocated using a pointer
	// This issue may or may not be detected during runtime.
	// Therefore, special caution is needed when using pointers to dynamically allocated memory
	// arr.pArr[1800] = 10;
	free(arr.pArr);

	return 0;
}
