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
						 // end - 마지막 데이터 다음
						 // Error

	vector<int>::iterator iter = vecInt.begin();
	iter += 4;
	//++iter; // End 넘어를 가리킬 수 없다.
	
	vecInt.pop_back();	

	// vector 는 동적배열 기반 자료구조로써, 맨 앞에 있는 데이터를 제거하는 기능은 비효율적이다.
	// 따라서 pop_front 기능을 직접적으로 제공하지는 않지만, iterator 를 활요하는 erase 함수는 제공한다.
	iter = vecInt.begin();
	vecInt.erase(iter);

	// 가리키던 대상이 삭제되버림, 해당 iterator 는 더이상 유효하지 않음
	//int Data = *iter;


	list<int> intList;

	intList.push_back(10);
	intList.push_back(20);
	intList.push_back(30);
	intList.push_back(40);

	intList.pop_front();

	// list 는 연결형리스트 기반 자료구조이기 때문에, 저장한 데이터 중에서 특정 인덱스에 접근할 수있는
	// [] 연산자나, at() 함수를 제공하지는 않지만, iterator 를 활용해서 
	// 특정 인덱스의 데이터에 접근할 수 있다.
	list<int>::iterator Liter = intList.begin();
	Liter = intList.erase(Liter);

	int Data = *Liter;
	// erase 사용 시 주의할 점
	// erase 입력으로 들어간 iterator 가 가리키는 요소를 제거하기 때문에,
	// 해당 iterator 는 더이상 유효하지 않은, 쓸 수 없는 iterator 가 되어버린다.


	intList.clear();

	// 1 에서 100까지 채움
	for (int i = 0; i < 100; ++i)
	{
		intList.push_back(i + 1);
	}
		

	// 60 이하 숫자는 제거
	// 61 부터 100까지 
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