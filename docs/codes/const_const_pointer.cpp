#include <stdio.h>

// Variable templates, constexpr, if constexpr ==> Compile-time optimization
// const, constexpr
// constant

int main()
{
	// Constant and variable
	int a = 10; // Initialization
	a = 100;    // Assignment

	// The value assigned to variable 'a' can only be known when the program runs.
	// The value inside a variable isn't fixed and can change at any time.
	// That's because it's a memory space that stores data.
	// On the other hand, constants are defined at the code level, and any number typed in code is already converted during compilation,
	// so the behavior is fixed.
	//scanf_s("%d", &a);

	// const
	// When declaring a variable, if you put const in front,
	// that variable becomes a constant.
	// A constant variable can be assigned a value only once at initialization.
	// Assigning a value after initialization is considered a syntax error since it's treated as reassignment.
	volatile const int iConst = 100; // Initialization
	//iConst = 200;          // Reassignment is a syntax error

	// The address type of a constant variable is a const pointer type.
	// To receive the address of a constant variable, you must use a const pointer.
	// A const pointer type means that the pointer (storing the address) cannot modify the original value it points to.
	volatile const int* pData = &iConst;
	//*pData = 200;

	int* pData1 = (int*) &iConst;
	*pData1 = 200;

	// Due to compiler optimization, it doesn¡¯t check the value of iConst,
	// and instead uses the initial value (100) recorded in the CPU register memory and assigns it to k.
	// So even if iConst was forcibly changed in the middle, k still receives the value 100 from the register.
	// If you want the changed value of iConst to reflect in k,
	// declare iConst with the volatile keyword to give it volatile properties
	// (prevent register optimization, always re-check the value of the variable before use)
	int k = iConst;

	// volatile : Volatile
	// A variable with this keyword is volatile (can easily change).
	// It prevents register optimization.

	// A stricter constant than const, compile-time constant
	// The exact value must be known at compile time. Cannot initialize with a variable.
	// A constexpr variable, when referenced or mentioned, is used directly with its initialized value at compile time.
	// It does not perform a check to retrieve the value from the variable.
	scanf_s("%d", &k);
	volatile const      int ConstParam = k;
	volatile constexpr  int iConstExpr = 100;

	int j = iConstExpr;

	int* pConstExpr = (int*)&iConstExpr;
	*pConstExpr = 200;

	int p = iConstExpr;

	return 0;
}