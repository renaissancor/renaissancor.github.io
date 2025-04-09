class CNewClass
{
private:
	int			m_i;
	const int	m_i2;

public:
	// Member functions marked const are safe to call on const pointers or references.
	// Getter-type functions should be marked const for const-safety.
	int GetInt() const
	{	
		return m_i;
	}

	int GetInt2() const
	{
		return m_i2;
	}

	void SetData(int _a)
	{
		m_i = _a;
	}

public:
	// Constructor - a special member function
	// Called when the object is created.
	// No return type, and function name must match the class name.
	CNewClass() 
		: m_i(200)    // Initializer list
		, m_i2(400)   // const variables can only be initialized in the initializer list
	{		
		// Assignment after construction (not valid for const members)
		// m_i = 100;
		// m_i2 = 200;
	}

	// Destructor - a special member function
	// Called when the object is destroyed.
	// No return type, and function name must match the class name, prefixed with ~.
	~CNewClass()
	{
	}

	// Every member function has an invisible local variable called 'this',
	// which holds the address of the calling object.
	// When referring to member variables/functions, `this->` is implicitly used.
};

int main()
{
	CNewClass my;

	int i = 0;
	i = my.GetInt();
	i = my.GetInt2();

	CNewClass* pNC = &my;
	i = pNC->GetInt();
	i = pNC->GetInt2();

	const CNewClass* pConstNC = &my;
	i = pConstNC->GetInt();
	i = pConstNC->GetInt2();

	my.CNewClass::SetData(200);

	CNewClass my2;
	my2.SetData(300);

	return 0;
}
