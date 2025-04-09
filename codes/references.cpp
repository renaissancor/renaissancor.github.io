int main()
{
	// C++
	// References
	// Similar in behavior to pointers

	int a = 0;

	// Pointer
	int* pInt = &a;
	*pInt = 100;

	// Reference
	int& Ref = a;
	Ref = 200;

	return 0;
}
