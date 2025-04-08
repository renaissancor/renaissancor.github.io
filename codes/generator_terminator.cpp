class CNewClass
{
private:
	int			m_i;
	const int	m_i2;

public:
	// const pointer or reference-safe member function
	// Get-type functions are marked const to allow calling with const pointers as well.
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
	// Constructor - member function
	// A function called when an object is created
	// No return type. Function name must match the class name.
	CNewClass()
		: m_i(200) // Initializer list
		, m_i2(400) // const-type variables can only be initialized in initializer list
	{
		// Assignment (not used here since initializer is used)
		//m_i = 100;
		//m_i2 = 200;
	}

	// Destructor - member function
	// A function called when an object is destroyed
	// No return type. Function name must match class name and be prefixed with ~
	~CNewClass()
	{
	}

	// Every member function has an invisible local variable called this.
	// It stores the address of the object that called the member function.
	// When accessing members inside member functions, this-> is implicitly used.
};

int main()
{
	CNewClass my;
	//my.m_i;
	//my.m_i2;

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
