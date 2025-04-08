#include "my_string.h"

void StringCopy(char* _Dest, const char* _Src)
{
	int idx = 0;
	while (_Src[idx] != 0)
	{
		_Dest[idx] = _Src[idx];
		++idx;
	}
	_Dest[idx] = _Src[idx];
}

int StringLen(const char* _Src)
{
	// At the end of the string, there is a 0 (\0) character.
	int Len = 0;
	while (_Src[Len] != 0)
	{
		++Len;
	}

	return Len;
}


void StringCat(char* _Dest, const char* _Src)
{
	int DestLen = StringLen(_Dest);

	int SrcLen = StringLen(_Src);

	for (int i = 0; i < SrcLen; ++i)
	{
		_Dest[i + DestLen] = _Src[i];
	}

	_Dest[DestLen + SrcLen] = 0;
}

// Decimal
void ItoA(char* _Dest, int _Num)
{
	// Check how many digits the input number has
	int Count = 0;
	int Result = _Num;
	while (true)
	{
		Result /= 10;
		++Count;
		if (Result == 0)
			break;
	}

	// Repeat for the number of digits, break down each digit into ones place,
	// convert each to its corresponding character, and store in array
	for (int i = 0; i < Count; ++i)
	{
		_Dest[Count - (i + 1)] = (_Num % 10) + 48;
		_Num /= 10;
	}

	_Dest[Count] = 0;
}

int AtoI(const char* _Src)
{
	// "2452"

	// Find the length of the string
	int Count = StringLen(_Src);

	// Repeat for the length of the string, convert each character to actual number
	// Convert characters to digits and accumulate with increasing decimal places
	int Value = 0;

	for (int i = 0; i < Count; ++i)
	{
		Value *= 10;
		Value += (_Src[i] - 48);
	}

	return Value;
}