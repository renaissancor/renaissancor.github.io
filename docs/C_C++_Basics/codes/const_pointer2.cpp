#include <stdio.h>


void DataOutput(const int* _Data)
{
	printf("%d\n", *_Data);

	// *_Data = 300;
}

int main()
{
	// const + pointer
	const int a = 10;
	// a = 100; // const variable cannot be modified


	// pointer is a variable that stores an address
	const int* pInt = &a;

	// const pointer cannot modify the target (original)
	// you can read the value from the target
	// *pInt = 100;
	int i = *pInt;

	// is const pointer modifiable?
	pInt = nullptr;


	// for the pointer variable itself to become a constant
	int j = 0;
	int* const pContInt = &j;
	// pContInt = nullptr;
	*pContInt = 200;

	// 1. When const is placed before *
	//  - becomes a read-only pointer, cannot modify the target (original)

	// 2. When const is placed after *
	//  - pointer variable itself becomes a constant. It can receive an address only once during initialization.

	// 3. When const is placed both before and after *
	//  - a read-only pointer which can only be initialized once
	const int* const pCCInt = &j;

	{
		// Data variable is const, so it cannot be modified after initialization
		// Address of Data is treated as const int*
		const int Data = 0;
		const int* pData = &Data;

		int* p = (int*)Data;
	}

	int k = 200;
	DataOutput(&k);

	return 0;
}
