#include "func.h"

static int g_Global = 0;

// External
extern int g_A;

int Add(int a, int b)
{
	g_Global = 10;
	g_A = 200;

	return a + b;
}

int Sub(int a, int b)
{
	return a - b;
}

int Mul(int a, int b)
{
	return a * b;
}

int Div(int a, int b)
{
	return a / b;
}

int Func()
{
	return 0;
}
