// Preprocessor
#define MY int
#define MAX_FLOOR 60

// 1. Simplify frequently used code
// 2. Represent code in readable text form
// 3. Simplify complex code patterns

#define ATT_UP		0x1
#define DEF_UP		0x2
#define DEX_UP		0x4
#define SPEED_UP	0x8
#define ATT_DOWN	0x10
#define DEF_DOWN	0x20
#define DEX_DOWN	0x40
#define SPEED_DOWN	0x80

// Define frequently used combinations of the above statuses
#define EVENT_POTION (ATT_UP | DEF_UP | DEX_UP | SPEED_UP)

int main()
{
	// Bitwise Operators
	// <<, >>
	// &, |, ^, ~
	// Only usable when the operands are integers
	// Operates at the bit level
	// << shifts all bits to the left
	// Each bit doubles, overall result becomes 2 times
	// Shift by 3 positions = multiplied by 2^3(8)
	int a = 2 << 3;

	// >> shifts bits to the right
	// Result is quotient divided by 2^n

	// Bitwise operation results may be adjusted based on positive/negative number representation system
	// To perform exact bit operations without correction, use unsigned data type
	unsigned char c = -1;
	c = c >> 1; // c >>= 1;
	c = 127;
	c = c << 1; // c <<= 1;

	// & (AND)
	unsigned char c1 = 10;		// 0000 1010
	unsigned char c2 = 2;		// 0000 0010
								// 0000 0010
	unsigned char c3 = c1 & c2;

	// | (OR)
	c1 = 10;		// 0000 1010
	c2 = 2;			// 0000 0010
	c3 = c1 | c2;	// 0000 1010

	// ~ inverts all bits (0 becomes 1, 1 becomes 0)
	c1 = 0;			// 0000 0000
	c1 = ~c1;       // 1111 1111

	// ^ (XOR) returns 0 if bits are the same, 1 if different
	c1 = 10;		// 0000 1010
	c2 = 2;			// 0000 0010
	c3 = c1 ^ c2;	// 0000 1000

	// If you want to assign the result of adding 2 to c3 back to c3, use +=
	c3 = c3 + 2;
	c3 += 2;

	// How to use bitwise operators
	// Character status
	// Each bit represents one status
	unsigned long long CharStat = 0;

	// 0x is hexadecimal notation
	CharStat |= ATT_UP;      // Turn on a specific bit
	CharStat |= DEF_UP;      // Turn on a specific bit
	CharStat |= DEX_UP;      // Turn on a specific bit
	CharStat |= SPEED_UP;    // Turn on a specific bit

	// Check if a specific bit is set
	if (CharStat & DEF_UP)
	{
		int a = 0;
	}

	// Toggle a specific bit using XOR (^)
	// If not set, it sets it; if set, it clears it
	if (CharStat & SPEED_UP)
	{
		CharStat ^= SPEED_UP;
	}

	// Unconditionally clear a specific bit
	CharStat &= ~SPEED_UP;

	// Use predefined bit combinations frequently
	CharStat |= EVENT_POTION;

	return 0;
}
