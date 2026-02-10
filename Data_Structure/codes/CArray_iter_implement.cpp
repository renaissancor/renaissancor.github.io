#include <vector>
#include <iostream>
using namespace std;

#include "CArray.h"

int main()
{
	// 데이터가 하나도 입력되지 않았을때 begin 을 호출하면, enditerator 가 반환된다.
	// begin 이랑 end 가 일치하는 상황
	{
		vector<int> vecInt;
		vector<int>::iterator veciter = vecInt.begin();

		if (veciter == vecInt.end())
		{
			int a = 0;
		}


		CArray<int> arrInt;
		CArray<int>::iterator arriter = arrInt.begin();

		if (arriter == arrInt.end())
		{
			int a = 0;
		}
	}	

	vector<int> vecInt;
	vecInt.push_back(1);
	vecInt.push_back(2);
	vecInt.push_back(3);
	cout << "vector 데이터 출력" << endl;
	vector<int>::iterator veciter = vecInt.begin();
	for (; veciter != vecInt.end(); ++veciter)
	{
		cout << *veciter << endl;
	}

	veciter = vecInt.begin();	
	int i = *(veciter++);

	CArray<int> arrInt;
	arrInt.push_back(1);
	arrInt.push_back(2);
	arrInt.push_back(3);
	cout << "CArray 데이터 출력" << endl;
	CArray<int>::iterator arrIter = arrInt.begin();
	for (; arrIter != arrInt.end(); ++arrIter)
	{
		cout << *arrIter << endl;
	}


	{
		// 전위 연산자, 후위연산자
		// ++, -- 
		CArray<int> arrInt;

		arrInt.push_back(1);
		arrInt.push_back(2);
		arrInt.push_back(3);

		CArray<int>::iterator arrIter = arrInt.begin();
		int value = *(arrIter++);

		int Data = *(arrIter);

		int i = 0;
		int j = i++;
	}




	return 0;
}
