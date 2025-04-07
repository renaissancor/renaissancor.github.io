// Structures in C
// Structures in C++
struct MyType
{
	// Member variables
	int		I;
	float	F;
};

struct NewType
{
	MyType  type;
	int		Age;
	int		Height;
};

typedef struct Player
{
	int MaxHP;
	int CurHP;
	int Att;
	int Def;
	int Dex;
	int Agi;
	int Int;
} PLAYER;

int main()
{
	struct Player p;
	PLAYER p1;

	// Structure (struct)
	// A data type created by the programmer
	// User-defined DataType
	struct MyType t1;
	MyType t;

	int size = sizeof(MyType);

	t.I = 100;
	t.F = 1.1111f;

	typedef int INT;
	typedef int SIZE;
	typedef int TIME;

	INT a = 0;
	SIZE b = 0;
	TIME c = 0;

	typedef struct Monster
	{
		int i;
		char c;
		short s;
	} MONSTER;

	Monster n = {};
	MONSTER m = {};

	return 0;
}
