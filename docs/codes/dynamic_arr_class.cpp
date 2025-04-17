
#include "Array.h"
#include "CArray.h"


int main()
{
	tArray arr = {};
	InitArr(&arr);

	PushBack(&arr, 10);
	PushBack(&arr, 20);
	PushBack(&arr, 30);


	CArray IntArr;

	IntArr.reserve(100);

	IntArr.push_back(10);
	IntArr.push_back(20);
	IntArr.push_back(30);




	return 0;
}