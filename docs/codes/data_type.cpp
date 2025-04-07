// Function
// Program Entry Point
int main()
{
	// C++, C, Java, C#, Objective C
	//			|
	//		Compiler
	//			V
	// Assembly Code (Machine Language), Binary

	// Data Types
	// Write values to memory space, perform operations,
	// Must be able to allocate space to store data in memory.
	// 1. Size
	// 2. Purpose

	// Data types are reserved keywords that describe the size and purpose of the memory space to store data.
	// Integer: bool(1), char(1 Byte, 8 bit), short(2), int(4), long(4), long long(8)
	// Floating point: float(4), double(8)

	// Variable (Parameter), Object (Instance)
	short My = 0; // Declare and initialize variable simultaneously
	My = 3.14;

	float Other = 0;
	Other = 3;

	// + - * / Arithmetic operators
	// = Assignment operator
	// == Comparison operator to check if left and right are equal

	// Integer data types
	// Binary
	// Decimal

	// signed: represents both negative and positive values
	// By default, if nothing is written before the data type, it is considered signed.
	// -128 ~ 127
	signed char sc = 0;

	// unsigned: represents only 0 and positive values
	// 0 ~ 255
	unsigned char uc = 0;

	// 1 byte can represent 256 different values (possibilities)

	// -128 ~ -1 , 0 ~ 127
	sc = 128;

	// 0 ~ 255
	uc = 128;

	// Two's complement method (bit calculation for negative numbers)
	// To get the bit representation of a negative number, invert all bits of the positive version and add 1.
	char c = 256; // 256 == 2^8

	unsigned char unchar = -1;

	// float, double
	// Floating point, IEEE

	// Integer and floating point have fundamentally different representation systems.
	// Integer representation <-> Floating point representation

	// float(4), double(8)

	int		iData = 20;     // Initialization
	float	fData = 3.66f; // Initialization

	// Type Casting
	float	fResult = (float)iData + fData; // Initialization

	fData = 118.625f; // Assignment
	fData = 118.624f; // Assignment

	// Operators
	// Assignment Operator
	// =
	// Distinguishing initialization and assignment

	// Increment/Decrement Operators
	// ++, -- increases or decreases value by 1
	// Prefix operator has the highest precedence
	// Postfix operator has the lowest precedence
	int Num = 0;
	++Num;
	--Num;
	Num++;
	Num--;

	int Integer = 100;
	Num = Integer++;

	// When used alone, there is no difference between prefix and postfix
	// When mixed with other operators, the overall result may differ.
	++Integer;
	Integer++;

	// Arithmetic Operators
	// + - *
	// / (division) (operands can be integer or float)
	// Division between integers returns only the quotient
	// Division between floats continues until exact result is obtained
	float	Div		= 10.f / 3.f;
	double	Div_d	= 10.0 / 3.0;
	int	iDiv	= 10 / 3;

	// % Remainder operator (only works with integer operands)
	int left = 10 % 3;

	// True or False
	// 0			: False
	// Any number other than 0: True, 1 is considered a representative of True

	// bool is a dedicated data type for storing true or false states
	bool b = 0;
	b = 120; // When a non-zero (true) value is stored in a bool variable, it is stored as 1
	char other = b;

	// Comparison Operators
	// ==, !=, <, >, <=, >=
	// Result of comparison is either true or false
	int IsSame = 10 >= 10;

	// Logical Operators
	// &&(And), ||(Or), !(Not)
	int IsTrue = 10 && 0;
	IsTrue = 1 || 0;
	IsTrue = !IsTrue;

	// Conditional Statements
	// if, else if, else
	// else if is checked only if if fails,
	// If one of if or else if conditions is true, others are not checked
	// If you want each condition to be evaluated independently, write each as an if statement

	// switch case

	int HP = 0;

	// If HP is greater than 80
	if (80 < HP)
	{
		int a = 0;
	}

	// If HP is greater than 60 and less than or equal to 80
	else if (60 < HP)
	{
		int a = 0;
	}

	else if (40 < HP)
	{
		int a = 0;
	}

	else if (20 < HP)
	{
		int a = 0;
	}

	else
	{
		int a = 0;
	}

	// Bitwise Operators
	// <<, >>
	// &, |, ^, ~

	return 0;
}
