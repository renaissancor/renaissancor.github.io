#include <stdio.h>

#include <iostream>
using namespace std;

int GetInt()
{
	int a = 100;
	return a;
}

void EndL(void)
{
	printf("\n");
}

void Test(void)
{
	printf("a");
}


// Function Overloading

// Operator Overloading
// + - * / % ++ --

class CMyDataType
{
private:
	int		m_Data;

public:
	void SetData(int _i)
	{
		m_Data = _i;
	}

	// Member function
	CMyDataType operator+(CMyDataType _Other)
	{
		CMyDataType data;

		data.m_Data = m_Data + _Other.m_Data;

		return data;
	}

	CMyDataType& operator << (int a)
	{
		printf("%d", a);

		return *this;
	}

	CMyDataType& operator << (float a)
	{
		printf("%f", a);

		return *this;
	}

	CMyDataType& operator << (void(*pFunc)(void))
	{
		pFunc();

		return *this;
	}

public:
	CMyDataType()
		: m_Data(0)
	{}

	~CMyDataType()
	{}
};

// Global function 
//CMyDataType operator+(CMyDataType _Left, CMyDataType _Right)
//{
//
//}

// Function Overloading

int main()
{
	int i = GetInt();

	int a = 100;
	int b = 200;
	int c = a + b;

	CMyDataType d1;
	d1.SetData(10);

	CMyDataType d2;
	d2.SetData(100);

	CMyDataType d3 = d1 + d2;

	d1 << 20 << 30 << 40 << EndL << 10 << 20 << 30;
		
	cout << 10 << 20 << 30 << endl;

	int k = 0;
	cin >> k;

	return 0;
}
