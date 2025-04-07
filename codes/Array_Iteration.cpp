// Preprocessor, includes external headers (declaration info)
#include <stdlib.h>
#include <stdio.h>

// Function
int main()
{
	// Local variables
	// Variables declared inside a function

	// When naming variables, avoid using duplicate names.
	// If there are two or more variables with the same name, it creates ambiguity about which one is being referenced.
	int a1 = 0;
	int a2 = 0;
	int a3 = 0;

	// You can create a new local scope inside a function.
	// Variables declared inside this scope are considered destroyed once the scope ends.
	// However, the actual memory space is not necessarily deallocated.
	{
		int a1 = 0;
		a1 = 10;
		{
			int a1 = 0;
		}
	}

	a1 = 10;

	// Array
	// 1. Elements in an array exist in contiguous memory locations.
	// 2. Array name represents the starting address
	//int Arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//int Arr[10] = {};
	int Arr[10] = { 1, 2,  };

	// Access using indexing
	Arr[0] = 10;
	Arr[1] = 20;
	Arr[9] = 100;

	int data = Arr[9];

	// Loops
	// Execute the same code multiple times.
	// Be cautious of infinite loops.
	// break; ==> Immediately exit the loop
	// continue; ==> Skip to the next iteration
	int Count = 0;
	while (Count < 4)
	{
		++Count;
	}

	int C = 0;
	while (1)
	{
		++C;
		if (C == 4)
		{
			break;
		}
	}

	// Initialization, condition, increment after each iteration
	for (int Count = 0; Count < 4; ++Count)
	{
		/*if (Object[Count]->IsMonster())
		{
			SetDamage(Object[Count]);
		}
		else
		{
			continue;
		}*/
	}

	// sizeof - can be used to determine the size of a datatype, variable, or array
	int size = sizeof(Arr);
	int ArrCount = sizeof(Arr) / sizeof(int);

	// Loop runs 10 times
	for (int i = 0; i < ArrCount; ++i)
	{
		Arr[i] = (i + 1) * 10;
		printf("Output\n");
	}

	// Nested loops
	int Num = 5;
	for (int j = 0; j < Num; ++j)
	{
		for (int i = 0; i < (j + 1); ++i)
		{
			printf("*");
		}
		printf("\n");
	}

	// Conditional statements
	// if, else if, else
	// switch case

	return 0;
}

// Assignment
// Print a balanced tree pattern of * characters
// EX:
//    *
//   * *
//  * * *
// * * * *
//* * * * *