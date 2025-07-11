#include <list>
#include <iostream>
using namespace std;


#include "CList.h"

int main()
{
	list<int> stlList;
	for (int i = 0; i < 10; ++i)
	{
		stlList.push_back(i+1);
	}
	
	cout << "List Data" << endl;
	list<int>::iterator iter = stlList.begin();
	for (iter; iter != stlList.end(); ++iter)
	{
		cout << *iter << endl;
	}




	CList<int> mylist;
	for (int i = 0; i < 10; ++i)
	{
		mylist.push_back(i + 1);
	}

	cout << "MyList Data" << endl;
	CList<int>::iterator myiter = mylist.begin();
	for (myiter; myiter != mylist.end(); ++myiter)
	{
		cout << *myiter << endl;
	}

	mylist.clear();	

	return 0;
}
