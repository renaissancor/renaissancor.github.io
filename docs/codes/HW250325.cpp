#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void StringCat(char *_Dest, const char *_Src)
{
	unsigned int idx_d = 0; 
	for (char* ptr = _Dest; *ptr != '\0'; ++ptr) {
		++idx_d;
	}

	unsigned int idx_s = 0;
	for (const char* ptr = _Src; *ptr != '\0'; ++ptr) {
		_Dest[idx_d] = _Src[idx_s]; 
		++idx_s; 
		++idx_d; 
	}

	_Dest[idx_d] = '\0'; 
}

void ItoA(char* _Dest, int _Num) 
{
	if (_Num == 0) {
		_Dest[0] = '0';
		_Dest[1] = '\0';
		return;
	}

	unsigned int cnt = 0; 
	int num = _Num; 

	char* ptr = _Dest; 
	while (num > 0) // num > 0 || num < 0
	{
		*ptr++ = (num % 10) + 48; // 48 == '0' 
		num /= 10; 
	}

	*ptr = 0; // EOL '\0' 

	for (char* start = _Dest, *end = ptr - 1; start < end; ++start, --end) {
		char tmp = *start;
		*start = *end;
		*end = tmp;
	}	
}

int AtoI(const char* _Src)
{
	int num = 0; 
	for (const char *ptr = _Src; *ptr != 0; ++ptr)
	{
		if (47 < *ptr && *ptr < 58)
		{
			num *= 10; 
			num += (int)(*ptr - 48); 
		} // '0' == 48, '\0' == 0 
		else
		{
			assert(0); 
		}
	}
	return num; 
}

int main() {
	char str[27];
	for (unsigned int i = 0; i < 26; ++i)
	{
		str[i] = 65 + i; // 'A' to 'Z'
	}
	str[26] = '\0';

	printf("Alphabet: %s\n", str);

	char num_str[6] = { '2', '3', '4', '5', '6', '\0' };
	printf("AtoI: %d\n", AtoI(num_str));

	char buffer[7];
	ItoA(buffer, 125789);
	printf("ItoA: %s\n", buffer);

	ItoA(buffer, 0);
	printf("ItoA(0): %s\n", buffer); 

	char cat_buf[64] = "Hello";
	StringCat(cat_buf, ", world!");
	printf("StringCat: %s\n", cat_buf);
	if (strcmp(cat_buf, "Hello, world!") == 0) {
		printf("StringCat passed\n");
	}
	else {
		printf("StringCat failed: got \"%s\"\n", cat_buf);
	}

	return 0;
}
