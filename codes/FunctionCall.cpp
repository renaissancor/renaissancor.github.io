void Test2()
{

}


void Test1()
{
	Test2();

}


int Pow(int _a, int _b)
{
	int Value = 1;

	for (int i = 0; i < _b; ++i)
	{
		Value *= _a;
	}

	return Value;
}


// Recursive function (Recursion, Recursive)
// A function structured to call itself again
// Caution - Beware of StackOverflow
//         - A termination condition must exist
// Can be easily used to implement features related to hierarchical structures
// Has good readability

// Recursive functions can be replaced with iterative structures
// Slower performance (optimization issues)

void NewFunc(int a)
{
	NewFunc(100);
}

int main()
{
	// Use debugging to understand how functions work
	int i = Pow(2, 4);
	i = Pow(10, 4);

	int c = 10;
	c = 20;

	// Function operation principle
	// Functions (code) written in C++ are equivalent to instructions
	// The memory space used during function execution is the Stack area

	// Memory space used by local variables
	// Stack memory area

	NewFunc(100);

	return 0;
}

// Debugging
// Ctrl + Shift + B  : Build
// F5                : Run in debug mode after build
//                     When paused at a breakpoint, shows the state just before executing the line
//                     Pressing F5 during debugging continues until the next breakpoint
//
// F9                : Create or remove a breakpoint at the cursor
// Ctrl + Shift + F9 : Remove all breakpoints

// F10               : Step over (procedure level)
// F11               : Step into (execute code)
