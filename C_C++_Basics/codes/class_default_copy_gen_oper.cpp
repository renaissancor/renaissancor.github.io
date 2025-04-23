
// -----------------------------------------------------------
// Class showing constructor overloading and default constructor
class MyClass
{
private:
    float m_Data;

public:
    void SetData(float _Data)
    {
        m_Data = _Data;
    }

    // Default constructor
    // If no constructor exists, the compiler generates one.
    // If any constructor exists, the default one won't be auto-generated.
    MyClass()
        : m_Data(0.f)
    {}

    // Constructor overloading
    MyClass(float _Data)
        : m_Data(_Data)
    {}

    // Custom copy constructor
    MyClass(const MyClass& _Other)
        : m_Data(_Other.m_Data)
    {}

    ~MyClass()
    {}
};

int main()
{
    // This is interpreted as a function declaration, NOT as an object creation:
    // MyClass data();
    // So instead, just do:
    MyClass data;

    // Use copy constructor
    MyClass my1;
    my1.SetData(1.1f);

    MyClass my2(my1); // Copy constructor

    return 0;
}
