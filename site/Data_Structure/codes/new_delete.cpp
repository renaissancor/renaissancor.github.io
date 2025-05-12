#include <iostream>
using namespace std;

class MyClass
{
private:
	int m_i;

public:
	MyClass()
		: m_i(0)
	{
		cout << "Constructor called" << endl;
	}
	~MyClass()
	{
		cout << "Destructor called" << endl;
	}
};

int main()
{
	// C-style dynamic allocation
	int* pData = (int*)malloc(sizeof(int) * 2);
	free(pData);

	// C++ dynamic allocation with constructor/destructor
	// Traditional malloc and free only allocate memory and free it,
	// so there's no way to know how that space is being used.
	// Therefore, when creating class-type objects on the heap,
	// constructors and destructors are not guaranteed to be called.

	// To solve this, the new and delete operators use templates
	// to allocate the appropriate size for the given type,
	// and automatically call the constructor and destructor.

	// Dynamic allocation and deallocation using C's malloc and free
	MyClass* pMy = (MyClass*)malloc(sizeof(MyClass));
	free(pMy);

	// Dynamic allocation and deallocation using C++'s new and delete
	MyClass* pNewMy = new MyClass;
	delete pNewMy;

	MyClass* pMyArr = new MyClass[10];

	pMyArr[0];
	pMyArr[1];
	pMyArr[9];

	delete[] pMyArr;

	return 0;
}
