#include <stdio.h>

int main()
{
	char arrData[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

	int* pData = (int*)arrData;

	pData += 2;
	char* pChar = (char*)pData;

	char a = *pChar;

	printf("%d", a);


	{
		char CharData = -1;

		unsigned char* pUChar = (unsigned char*) &CharData;
		char* pChar = &CharData;

		int intData = 0;
		intData = *pUChar;
		intData = *pChar;

		printf("%d", intData);
	}

	return 0;
}