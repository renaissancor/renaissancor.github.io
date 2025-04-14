
#include <iostream>
#include "CList.h"

int main()
{
    std::cout << "Hello World!\n";

	CList<int> c_list;

	for (int i = 0; i < 10; ++i)
	{
		c_list.push_back(i);
	}

	for (CList<int>::iterator iter = c_list.begin(); iter != c_list.end(); ++iter)
	{
		std::cout << *iter << " ";
	} 
	std::cout << std::endl;

	c_list.reverse(); 

	for (CList<int>::iterator iter = c_list.begin(); iter != c_list.end(); ++iter)
	{
		std::cout << *iter << " ";
	}
	std::cout << std::endl;

    return 0; 
}