#include <stdlib.h>
#include <stdio.h>

// Variables
// Local variable - function, stack
// Global variable - data
// Static variable - data
// External variable - data
int global = 10;
int g_Arr[10] = {};

// Custom data type
struct MyDatType
{
	int		arr[2];
	float	fArr[3];
};


int* Test()
{
	static int iStatic = 0;

	++global;
	++iStatic;

	return &iStatic;
}


// Memory areas
// 1. Stack area
// 2. Data area - created at program start, released at termination
// 3. ROM (code, initialized data (strings), constants)
// 4. Heap - memory area allocated through dynamic memory allocation

// As of now, the operation of the heap memory area has nothing to do with the heap data structure.
// Heap data structure: a structure for quickly searching max/min values

int main()
{
	// It's possible to access and modify the value by receiving the address of a static variable declared inside Test function
	int* pInt = Test();
	*pInt = 100;
	Test();

	// The static variable itself cannot be referred to in another function, but accessing it via address is allowed
	//++iStatic;
	++global;

	// Dynamic memory allocation
	// The act of allocating a memory space in the heap memory area as much as the user (programmer) wants

	// Necessity of dynamic allocation
	// Static binding
	// Memory size or function call is fixed at compile time

	// Using dynamic allocation allows allocating as much memory as desired during runtime
	// It allows flexible (dynamic) response to changes

	int Value = 10;
	int iArr[10];

	MyDatType data = {};
	data.arr[0];
	data.arr[1];
	data.fArr[0];
	data.fArr[1];
	data.fArr[2];


	return 0;
}
