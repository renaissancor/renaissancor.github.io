#include <vector>
#include <iostream>
using namespace std;

#include "CArray.h"

int main()
{
	// �����Ͱ� �ϳ��� �Էµ��� �ʾ����� begin �� ȣ���ϸ�, enditerator �� ��ȯ�ȴ�.
	// begin �̶� end �� ��ġ�ϴ� ��Ȳ
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
	cout << "vector ������ ���" << endl;
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
	cout << "CArray ������ ���" << endl;
	CArray<int>::iterator arrIter = arrInt.begin();
	for (; arrIter != arrInt.end(); ++arrIter)
	{
		cout << *arrIter << endl;
	}


	{
		// ���� ������, ����������
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
