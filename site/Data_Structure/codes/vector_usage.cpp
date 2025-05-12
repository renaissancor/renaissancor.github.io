#include <vector>
using namespace std;

#include "CArray.h"

int main()
{
	vector<int> vecInt;
	//vecInt.reserve(10); // Reserve memory in advance for future additions
	vecInt.resize(2000); // Immediately allocate memory and fill with data	

	vecInt.push_back(10);
	vecInt.push_back(20);
	vecInt.push_back(30);
	vecInt.push_back(40);
	vecInt.push_back(50);

	//vecInt.resize(10);
	vecInt.clear();

	{
		vector<int> vecTemp;
		vecTemp = vecInt;
		vecInt.swap(vecTemp);
	}

	int Size = vecInt.size();
	int Cap = vecInt.capacity();

	CArray<int> intArr;
	intArr.push_back(10);
	intArr.push_back(20);
	intArr.push_back(30);

	CArray<int> OtherArr;
	OtherArr = intArr;

	int Data = OtherArr[0];
	Data = OtherArr[1];
	Data = OtherArr[2];

	intArr.clear();
	{
		CArray<int> temp;
		intArr.swap(temp);
	}
	return 0;
}
