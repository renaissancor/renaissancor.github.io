

// 함수 템플릿
template<typename A>
A Add(A a, A b)
{
	A c = a + b;

	return c;
}


template<int Size>
void SetData(int (&_Arr)[Size])
{
	for (int i = 0; i < Size; ++i)
	{
		_Arr[i] = i + 1;
	}
}

void SetData(int* _Arr, int _Size)
{
	for (int i = 0; i < _Size; ++i)
	{
		_Arr[i] = i + 1;
	}
}


int main()
{
	int a = Add(10, 20);

	float f = Add(2.1f, 1.9f);

	short s1 = 10, s2 = 30;
	short s3 = Add(s1, s2);

	int iArr[20] = {};
	SetData(iArr);

	int Arr[40];
	SetData(Arr);

	return 0;
}