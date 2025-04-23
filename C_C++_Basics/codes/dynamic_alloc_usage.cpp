#include <stdlib.h>

// Dynamic Allocation
// Heap Area

// F12
int main()
{
	// Memory Allocate
	//int* pData = (int*)malloc(4);
	//*pData = 10;
	//float* pData = (float*)malloc(4);
	//*pData = 10.f;

	// Dynamic allocation function malloc
	// Allocates memory in the heap area as much as the input size (in bytes), then returns the start address.
	// The return type is void* because malloc doesn't know how the caller will use the allocated space.


	// free()
	// A function to free memory allocated by dynamic allocation

	// memory leak
	// If memory is dynamically allocated and the program ends without freeing that space,
	// it causes a memory leak.
	void* pData = malloc(200);
	free(pData);


	return 0;
}
