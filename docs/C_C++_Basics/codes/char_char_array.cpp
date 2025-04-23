#include <stdlib.h>
#include <iostream>

int main()
{
	// Character == Number
	// char - 1 byte, integer type, used to represent characters
	char c = 'c';

	// wchar_t - 2 bytes, integer type, used to represent characters
	wchar_t wc = 100;
	short s = 100;
	int i = 99;

	// ASCII table
	// A specific number corresponds to a specific character
	// '' is used to express a character
	// "" is used to express a string
	// A string means a sequence of characters.
	// To be a string, memory space must be continuous.
	// A string must end with a null character.

	// Instead of inserting characters into each element of the array manually,
	// it's better to initialize the array all at once using a string literal (only works with character types)
	char String[10] = "jeehoon";
	/*String[0] = 'j';
	String[1] = 'e';
	String[2] = 'e';
	String[3] = 'h';
	String[4] = 'o';
	String[5] = 'o';
	String[6] = 'n';
	String[7] = '\0';*/

	// Initializing each array element with characters is only allowed for character types like char and wchar_t.
	// Using a semantically equivalent 2-byte type like short is not allowed for this purpose.
	short szShort[10] = { 72, 101, 108, 108, 111, };
	wchar_t szWString[10] = L"Hello";

	// Numbers and numeric characters are different.
	char Num = 123;

	char NumStr[5] = {};
	NumStr[0] = '1';
	NumStr[1] = 32; // space character
	NumStr[2] = '3';
	NumStr[3] = '4';
	NumStr[4] = '\0';

	return 0;
}
