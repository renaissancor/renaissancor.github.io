#include <stdio.h>

// 1. Local Variable
// Variable declared inside a function
// Stack memory region

// 2. Global Variable
// Variable declared outside of functions
// Data segment
// Created at program start, destroyed at program end ==> Exists throughout program execution
// Name duplication not allowed, creates ambiguity as to which variable is being referenced
int g_A = 0;

// 3. Static Variable
// Uses the data segment (characteristics of a global variable)
// Does not move from where it is declared
//  - 1. If declared inside a function - can only be accessed within that function
//  - 2. If declared as a global variable, used in case of modular implementation
//  - 3. If declared inside a class


// 4. External Variable
// - Global variable, when doing modular implementation, a way to declare global variables accessible from all files
//   To use a global variable declared in a specific cpp file from another cpp file, use the extern keyword for forward declaration


// Modular implementation




int Test()
{
	int arr[100] = {};

	int k = 0;

	int g_A = 0;

	// If there is a local variable with the same name as a global variable,
	// The local variable takes precedence
	g_A = 100;

	// Static variable
	// A dedicated global variable that can only be accessed inside the Test function
	static int StaticParam = 0;
	++StaticParam;

	// Scope resolution operator
	// Changes scope target to global, refers to the global variable g_A instead of the local one
	++::g_A;

	return ::g_A;
}

int main()
{
	int k = 0;


	// Local variable
	// Cannot dynamically respond
	// Number of array elements or local variables cannot be increased during runtime
	// Array size must be a constant
	// If array size is a variable, the size of the stack (function) cannot be calculated
	int Arr[100] = {};

	int Size = 10;

	//scanf_s("%d", &Size);

	int NewArray[100] = {};

	g_A = 20;

	Test();

	Test();

	Test();

	int Count = Test();

	return 0;
}
