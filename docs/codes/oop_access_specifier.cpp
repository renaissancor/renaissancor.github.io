#include "List.h"

// C -> C++
// Class

// struct in C
// class, struct in C++

// class, struct: User-defined data types

// Features added to C++ Class
// Object-Oriented Language
// 1. Encapsulation (also possible in C struct)

// Additional features in C++ class
// 2. Information hiding (ability to control visibility of member variables)
// 3. Inheritance
// 4. Polymorphism
// 5. Abstraction (Abstract Class)

// In C++, class and struct both have the same capabilities.
// If no access specifier is explicitly written,
// the default is: struct = public, class = private

// C++ builds on top of C
struct MyData
{
public: // Access specifier
	int iData;
};

class CMyData
{
private: // Prevent external access
	int iData;
};

// public
// Members are fully accessible from outside

// private
// Hidden from outside, except within member functions

int main()
{
	MyData data = {};
	data.iData = 0;

	CMyData CData = {};
	//CData.iData = 0;

	tList mylist = {};
	PushBack(&mylist, 100);

	mylist.pHead = nullptr;
	mylist.Size = 1000000000;

	return 0;
}
