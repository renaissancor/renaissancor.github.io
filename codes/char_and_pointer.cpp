#include "MyString.h"

int main()
{
	int a = 10;
	char string[10] = "abcdef";
	string[0] = 'z';

	// Receiving the address of a string
	// The string literal "" means the address where the string is stored
	// const * type
	const char* pString = "abcdef";
	//pString[0] = 'z';

	char Other[20] = "abcdef";
	char Other2[20] = "abcdef";

	Other[1] = 'k';
	Other2[4] = 'q';

	// Since identical strings are stored only once in ROM,
	// and for safe use during initialization or as data,
	// strings written this way should not be modified.
	// They are stored in ROM, and you can get the address with the "" expression.
	// Trying to modify the string through this address will cause a runtime error
	// because it attempts to modify the ROM area.

	char szSrc[100] = "Copy";
	char szDest[100] = "";

	StringCopy(szDest, szSrc);

	char szSrc1[100] = "defg";
	char szDest1[100] = "abc";
	szDest1[8] = 'l';
	szDest1[9] = 'k';

	int Len = StringLen(szSrc1);

	StringCat(szDest1, szSrc1);
	// szSrc1 == "def"
	// szDest1 == "abcdef"

	char szNum[100] = {};
	ItoA(szNum, 72543);
	// szNum == "72543";

	int Value = AtoI("2452");
	// Value == 2452

	return 0;
}
