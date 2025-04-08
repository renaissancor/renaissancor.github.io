#include "List.h"

#pragma pack(1)

// Due to memory alignment rules,
// memory is aligned based on the size of the largest member in the struct.
struct tMyStruct
{
	int		i;
	int		i2;
	short	s;
	char    c;
};

int main()
{
	tMyStruct my = {};

	int size = sizeof(tMyStruct);

	return 0;
}
