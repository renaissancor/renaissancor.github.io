#include "CArray.h"

#include <vector>
#include <list>
using namespace std;

struct tMyStruct
{
	int iarr[10];
};

int main()
{
	tMyStruct st1 = {};
	st1.iarr[0] = 10;

	tMyStruct st2 = {};
	st2.iarr[1] = 100;

	CArray<tMyStruct> ArrData;
	ArrData.push_back(st1);
	ArrData.push_back(st2);

	tMyStruct temp = ArrData.at(0);
	temp = ArrData.at(1);

	CArray<int> arrInt;
	arrInt.push_back(10);
	arrInt.push_back(20);
	arrInt.push_back(30);

	int data = arrInt.at(2);
	arrInt.at(2) = 40;
	data = arrInt.at(2);

	arrInt[0] = 100;
	data = arrInt[0];

	// ==============
	// Comparison with vector
	// ==============
	vector<int> vecInt;
	vecInt.push_back(10);
	vecInt.push_back(20);
	vecInt.push_back(30);

	int Val = vecInt.at(0);
	Val = vecInt.at(1);
	Val = vecInt[0]; // same as vecInt.at(0)

	int iArr[10] = {};
	iArr[1] = 0;

	//vecInt.reserve(100);
	//vecInt.resize(100);
	//vecInt.swap();
	//vecInt.front();
	//vecInt.back();


	// ===========
	// Usage of list
	// Linked list, data is stored in nodes, and the nodes point to each other
	// ===========
	list<int> intList;
	intList.push_back(10);
	intList.push_back(20);
	intList.push_back(30);

	return 0;
}
