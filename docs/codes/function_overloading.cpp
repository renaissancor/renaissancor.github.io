int g_Data = 0;


// Function Overriding
// - Used in inheritance
// - Replaces the parent class's function in the child class

// Function Overloading
// - Multiple functions with the same name can be declared
// - Cannot be distinguished by return type
// - Distinguished only by number and type of parameters
// - When using default parameters, be careful as it may cause ambiguity in distinguishing overloaded functions
int Test(int _a)
{
	return 10;
}

void Test(float _f)
{

}

void Test(int _a, int _b = 0)
{

}

int main()
{	
	Test(1.1f);

	Test(10, 20);

	return 0;
}