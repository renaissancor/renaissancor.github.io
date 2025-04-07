// Pointer
// - A data type that stores an address
// * A variable declared as a pointer type takes up 8 bytes

// Pointer variable
// - A variable that stores an address


void MemSet(void* _pData, int _Size);
void MemMove(void* _pData, int _Size, void* _Src);

int main()
{
	// Memory addresses are handled in units of integers
	// One address = 1 byte

	// 4 bytes, integer
	int i = 0;
	char c = 0;
	short s = 0;

	// 8 bytes, variable to store an address
	int*  pI = 0;
	char* pC = 0;
	short* pS = 0;

	// Prepending & to a variable gives its starting address
	// Store the address of another variable in a pointer variable
	pI = &i;
	pC = &c;
	pS = &s;

	// Prepending * to a pointer variable accesses the address stored in it (dereferencing)
	*pI = 10;
	*pC = 20;
	*pS = 30;

	// Reason for attaching data type to * when declaring a pointer
	// The address in a pointer is a starting address
	// Because we can't know what data is at that address,
	// we declare what kind of data the pointer will point to in advance
	// The given address is interpreted as the predefined data type

	// Forced casting of pointers
	// You can forcefully store the address of a different type into a pointer
	pI = (int*) &c;
	pC = (char*)&i;

	// void pointer
	// Pointer - variable that stores an address
	// void type means "no type"; it doesn't interpret the original data of the stored address
	// void pointers cannot access the original data
	// Used purely to store address values
	void* pVoid = 0;

	pVoid = &i;
	pVoid = &c;
	pVoid = &s;	
	
	// You can't assign a void* to a regular pointer
	// Because we can't know what type of variable the void* originally pointed to
	//pS = pVoid;


	// Ctrl + Shift + Space to check function declarations
	// Array name = starting address
	// Array memory is contiguous
	short arr[10];

	// Call function to initialize memory starting from given variable to 0 for given size
	MemSet(arr, 20);

	int k;
	MemSet(&k, sizeof(int));

	// Call function to move data to another location
	int iArr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int iArrOther[10] = {};
	MemMove(iArrOther, sizeof(int) * 10, iArr);


	// Pointers and arrays
	// Based on the pointer type, the step size for address increment is the size of the type
	int* pInt = 0;
	pInt += 1;

	short* pShort = 0;
	pShort += 1;

	// Array indexing is address arithmetic
	// Means accessing the address offset by the index from the base address
	// address[N] == *(address + N)
	int iTest[5] = {};

	iTest[0] = 1;
	iTest[1] = 2;
	iTest[2] = 3;
	iTest[3] = 4;
	iTest[4] = 5;

	pInt = iTest;

	pInt[0] = 1;// *(pInt + 0) = 1; 
	pInt[1] = 2;// *(pInt + 1) = 2; 
	pInt[2] = 3;// *(pInt + 2) = 3; 
	pInt[3] = 4;// *(pInt + 3) = 4; 
	pInt[4] = 5;// *(pInt + 4) = 5; 

	return 0;
}
