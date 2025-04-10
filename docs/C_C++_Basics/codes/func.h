#pragma once

int Add(int a, int b);
int Sub(int a, int b);
int Mul(int a, int b);
int Div(int a, int b);
int Func();

// All files that include func.h will be aware of the existence of the global variable int g_A; (forward declaration)
extern int g_A;

// Each file that includes func.h will get its own static global variable
static int g_NewStatic = 0;

// For every file that includes func.h, a global variable with the same name will be created, leading to link errors
//int g_GlobalParam = 0;
