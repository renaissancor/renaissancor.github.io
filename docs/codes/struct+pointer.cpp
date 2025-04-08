struct MyType
{
	int		i;
	short	s;
};

int main()
{
	float f = 0.f;
	float* pF = &f;
	*pF = 1.11f;

	MyType type = {};
	type.i = 10;
	type.s = 20;

	MyType* pType = &type;
	(*pType).i = 30;
	(*pType).s = 40;

	// Same expression
	pType->i = 50;
	pType->s = 60;


	// A pointer variable is a variable that stores an address
	// Meaning of the data type that comes before * when declaring a pointer
	// Size of a pointer variable is 8 bytes
	int*  pInt  = nullptr;
	char* pChar = nullptr;

	int Size = sizeof(int*);

	return 0;
}