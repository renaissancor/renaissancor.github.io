#include <vector>
#include <list>
using namespace std;


#include "CArray.h"

int main()
{
	vector<int> vecInt;

	vecInt.push_back(1); // 0
	vecInt.push_back(2); // 1
	vecInt.push_back(3); // 2
	vecInt.push_back(4); // 3
						 // end - ������ ������ ����
						 // Error

	vector<int>::iterator iter = vecInt.begin();
	iter += 4;
	//++iter; // End �Ѿ ����ų �� ����.
	
	vecInt.pop_back();	

	// vector �� �����迭 ��� �ڷᱸ���ν�, �� �տ� �ִ� �����͸� �����ϴ� ����� ��ȿ�����̴�.
	// ���� pop_front ����� ���������� ���������� ������, iterator �� Ȱ���ϴ� erase �Լ��� �����Ѵ�.
	iter = vecInt.begin();
	vecInt.erase(iter);

	// ����Ű�� ����� �����ǹ���, �ش� iterator �� ���̻� ��ȿ���� ����
	//int Data = *iter;


	list<int> intList;

	intList.push_back(10);
	intList.push_back(20);
	intList.push_back(30);
	intList.push_back(40);

	intList.pop_front();

	// list �� ����������Ʈ ��� �ڷᱸ���̱� ������, ������ ������ �߿��� Ư�� �ε����� ������ ���ִ�
	// [] �����ڳ�, at() �Լ��� ���������� ������, iterator �� Ȱ���ؼ� 
	// Ư�� �ε����� �����Ϳ� ������ �� �ִ�.
	list<int>::iterator Liter = intList.begin();
	Liter = intList.erase(Liter);

	int Data = *Liter;
	// erase ��� �� ������ ��
	// erase �Է����� �� iterator �� ����Ű�� ��Ҹ� �����ϱ� ������,
	// �ش� iterator �� ���̻� ��ȿ���� ����, �� �� ���� iterator �� �Ǿ������.


	intList.clear();

	// 1 ���� 100���� ä��
	for (int i = 0; i < 100; ++i)
	{
		intList.push_back(i + 1);
	}
		

	// 60 ���� ���ڴ� ����
	// 61 ���� 100���� 
	for (Liter = intList.begin(); Liter != intList.end();)
	{
		if (*Liter <= 60)
		{
			Liter = intList.erase(Liter);
		}
		else
		{
			++Liter;
		}
	}

	return 0;
}