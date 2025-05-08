// Forward declaration
typedef char CHAR;

// Function forward declaration
//int Add(int a, int b);
//int Sub(int a, int b);
//int Mul(int a, int b);
//int Div(int a, int b);

#include "func.h"

// Global variable
// Global variables must not have duplicate names in the same file or other files.

// Static variable
//  - When the static keyword is added to a global variable,
//    it becomes a file-local global variable that can only be used in this file (cpp).
static int g_Global = 0;

// Create a global variable that can be used in all files
int g_A = 0;



int main()
{
	// Static variable
	static int iStatic = 0;

	g_Global = 200;
	//g_A = 100;

	int i = 0;
	
	i = Add(1, 9);
	i = Sub(1, 9);
	i = Mul(1, 9);
	i = Div(1, 9);
	Func();

	return 0;
}