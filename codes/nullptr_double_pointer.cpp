// C style true, false 
// Since there are no built-in keywords in C, we use macros instead
#define TRUE 1;
#define FALSE 0;


// You can access a specific function's local variable from another function using a pointer variable
void Test(int* _Data)
{
	*_Data = 10;
}

// Double pointer
void Test(int** _Data)
{
	*_Data = nullptr;
}

int main()
{
	int i = 0;
	Test(&i);

	int* pInt = (int*)1;
	Test(&pInt);

	// nullptr pointer
	int* pData = nullptr; // Address 0 means it points to nothing
	pData = nullptr;

	//int Data = nullptr;
	//Data = 0;
	
	// In pointer concept, 0 means pointing to nothing
	// There is a special keyword nullptr to set 0 to pointer variables
	// It can only be used when assigning values to pointer variables

	true; // Means 1
	false; // Means 0

	bool IsTrue = TRUE;

	int a = true;
	a = false;

	return 0;
}